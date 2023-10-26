package org.apache.felix.dm.tracker;
/*
 * Copyright (c) OSGi Alliance (2007, 2008). All Rights Reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.apache.felix.dm.impl.SerialExecutor;

/**
 * Abstract class to track items. If a Tracker is reused (closed then reopened),
 * then a new AbstractTracked object is used. This class acts a map of tracked
 * item -> customized object. Subclasses of this class will act as the listener
 * object for the tracker. This class is used to synchronize access to the
 * tracked items. This is not a public class. It is only for use by the
 * implementation of the Tracker class.
 * 
 * @ThreadSafe
 * @version $Revision: 5871 $
 * @since 1.4
 */
@SuppressWarnings({"rawtypes", "unchecked"})
abstract class AbstractTracked {
	/* set this to true to compile in debug messages */
	private static final boolean		DEBUG	= false;

	/**
	 * Map of tracked items to customized objects.
	 * 
	 * @GuardedBy this
	 */
	private Map			tracked;

	/**
	 * Modification count. This field is initialized to zero and incremented by
	 * modified.
	 * 
	 * @GuardedBy this
	 */
	private int					trackingCount;

	/**
	 * List of items in the process of being added. This is used to deal with
	 * nesting of events. Since events may be synchronously delivered, events
	 * can be nested. For example, when processing the adding of a service and
	 * the customizer causes the service to be unregistered, notification to the
	 * nested call to untrack that the service was unregistered can be made to
	 * the track method.
	 * 
	 * Since the ArrayList implementation is not synchronized, all access to
	 * this list must be protected by the same synchronized object for
	 * thread-safety.
	 * 
	 * @GuardedBy this
	 */
	private final List			adding;

	/**
	 * true if the tracked object is closed.
	 * 
	 * This field is volatile because it is set by one thread and read by
	 * another.
	 */
	volatile boolean			closed;

	/**
	 * Initial list of items for the tracker. This is used to correctly process
	 * the initial items which could be modified before they are tracked. This
	 * is necessary since the initial set of tracked items are not "announced"
	 * by events and therefore the event which makes the item untracked could be
	 * delivered before we track the item.
	 * 
	 * An item must not be in both the initial and adding lists at the same
	 * time. An item must be moved from the initial list to the adding list
	 * "atomically" before we begin tracking it.
	 * 
	 * Since the LinkedList implementation is not synchronized, all access to
	 * this list must be protected by the same synchronized object for
	 * thread-safety.
	 * 
	 * @GuardedBy this
	 */
	private final LinkedList	initial;
	
	private final SerialExecutor m_executor = new SerialExecutor(null); 

	/**
	 * AbstractTracked constructor.
	 */
    AbstractTracked() {
	    this.tracked = new HashMap();
	    trackingCount = 0;
	    adding = new ArrayList(6);
	    initial = new LinkedList();
	    closed = false;
	}
	
	void setTracked(HashMap map) {
	    this.tracked = map;
	}

	/**
	 * Set initial list of items into tracker before events begin to be
	 * received.
	 * 
	 * This method must be called from Tracker's open method while synchronized
	 * on this object in the same synchronized block as the add listener call.
	 * 
	 * @param list The initial list of items to be tracked. <code>null</code>
	 *        entries in the list are ignored.
	 * @GuardedBy this
	 */
    void setInitial(Object[] list) {
		if (list == null) {
			return;
		}
		int size = list.length;
		for (int i = 0; i < size; i++) {
			Object item = list[i];
			if (item == null) {
				continue;
			}
			if (DEBUG) {
				System.out.println("AbstractTracked.setInitial: " + item); //$NON-NLS-1$
			}
			initial.add(item);
		}
	}

	/**
	 * Track the initial list of items. This is called after events can begin to
	 * be received.
	 * 
	 * This method must be called from Tracker's open method while not
	 * synchronized on this object after the add listener call.
	 * 
	 */
	void trackInitial() {
		while (true) {
			Object item;
			synchronized (this) {
				if (closed || (initial.size() == 0)) {
					/*
					 * if there are no more initial items
					 */
					return; /* we are done */
				}
				/*
				 * move the first item from the initial list to the adding list
				 * within this synchronized block.
				 */
				item = initial.removeFirst();
				if (tracked.get(item) != null) {
					/* if we are already tracking this item */
					if (DEBUG) {
						System.out
								.println("AbstractTracked.trackInitial[already tracked]: " + item); //$NON-NLS-1$
					}
					continue; /* skip this item */
				}
				if (adding.contains(item)) {
					/*
					 * if this item is already in the process of being added.
					 */
					if (DEBUG) {
						System.out
								.println("AbstractTracked.trackInitial[already adding]: " + item); //$NON-NLS-1$
					}
					continue; /* skip this item */
				}
				adding.add(item);
				final AbstractCustomizerActionSet actionSet = trackAdding(item, null);
				m_executor.schedule(new Runnable() {

					@Override
					public void run() {
						actionSet.execute();
						
					}
					
				});
			}
			if (DEBUG) {
				System.out.println("AbstractTracked.trackInitial: " + item); //$NON-NLS-1$
			}
		}
	}

	/**
	 * Called by the owning Tracker object when it is closed.
	 */
	void close() {
		closed = true;
	}

	abstract AbstractCustomizerActionSet createCustomizerActionSet();
	
	/**
	 * Begin to track an item.
	 * 
	 * @param item Item to be tracked.
	 * @param related Action related object.
	 */
	AbstractCustomizerActionSet track(final Object item, final Object related) {
		final Object object;
		final AbstractCustomizerActionSet actionSet = createCustomizerActionSet();
		synchronized (this) {
			if (closed) {
				return actionSet;
			}
			object = tracked.get(item);
			if (object == null) { /* we are not tracking the item */
				if (adding.contains(item)) {
					/* if this item is already in the process of being added. */
					if (DEBUG) {
						System.out
								.println("AbstractTracked.track[already adding]: " + item); //$NON-NLS-1$
					}
					return actionSet;
				}
				adding.add(item); /* mark this item is being added */
			}
			else { /* we are currently tracking this item */
				if (DEBUG) {
					System.out
							.println("AbstractTracked.track[modified]: " + item); //$NON-NLS-1$
				}
				modified(); /* increment modification count */
			}
		}

		if (object == null) { /* we are not tracking the item */
			actionSet.appendActionSet(trackAdding(item, related));
		}
		else {
			/* Call customizer outside of synchronized region */
			actionSet.addCustomizerModified(item, related, object);
			/*
			 * If the customizer throws an unchecked exception, it is safe to
			 * let it propagate
			 */
		}
		return actionSet;
	}

	/**
	 * Common logic to add an item to the tracker used by track and
	 * trackInitial. The specified item must have been placed in the adding list
	 * before calling this method.
	 * 
	 * @param item Item to be tracked.
	 * @param related Action related object.
	 */
	private AbstractCustomizerActionSet trackAdding(final Object item, final Object related) {
		final AbstractCustomizerActionSet actionSet = createCustomizerActionSet();
		if (DEBUG) {
			System.out.println("AbstractTracked.trackAdding: " + item); //$NON-NLS-1$
		}
		Object object = null;
		boolean becameUntracked = false;
		/* Call customizer outside of synchronized region */
		try {
			object = customizerAdding(item, related);
			/*
			 * If the customizer throws an unchecked exception, it will
			 * propagate after the finally
			 */
		}
		finally {
			boolean needToCallback = false;
			synchronized (this) {
				if (adding.remove(item) && !closed) {
					/*
					 * if the item was not untracked during the customizer
					 * callback
					 */
					if (object != null) {
						tracked.put(item, object);
						modified(); /* increment modification count */
						notifyAll(); /* notify any waiters */
						needToCallback = true; /* marrs: invoke added callback */
					}
				}
				else {
					becameUntracked = true;
				}
			}
			if (needToCallback) {
				actionSet.addCustomizerAdded(item, related, object);
			}
		}
		/*
		 * The item became untracked during the customizer callback.
		 */
		if (becameUntracked && (object != null)) {
			if (DEBUG) {
				System.out
						.println("AbstractTracked.trackAdding[removed]: " + item); //$NON-NLS-1$
			}
			/* Call customizer outside of synchronized region */
			actionSet.addCustomizerRemoved(item, related, object);
			/*
			 * If the customizer throws an unchecked exception, it is safe to
			 * let it propagate
			 */
		}
		return actionSet;
	}

	/**
	 * Discontinue tracking the item.
	 * 
	 * @param item Item to be untracked.
	 * @param related Action related object.
	 */
	AbstractCustomizerActionSet untrack(final Object item, final Object related) {
		AbstractCustomizerActionSet actionSet = createCustomizerActionSet();
		final Object object;
		synchronized (this) {
			if (initial.remove(item)) { /*
										 * if this item is already in the list
										 * of initial references to process
										 */
				if (DEBUG) {
					System.out
							.println("AbstractTracked.untrack[removed from initial]: " + item); //$NON-NLS-1$
				}
				return actionSet; /*
						 * we have removed it from the list and it will not be
						 * processed
						 */
			}

			if (adding.remove(item)) { /*
										 * if the item is in the process of
										 * being added
										 */
				if (DEBUG) {
					System.out
							.println("AbstractTracked.untrack[being added]: " + item); //$NON-NLS-1$
				}
				return actionSet; /*
						 * in case the item is untracked while in the process of
						 * adding
						 */
			}
			object = tracked.remove(item); /*
											 * must remove from tracker before
											 * calling customizer callback
											 */
			if (object == null) { /* are we actually tracking the item */
				return actionSet;
			}
			modified(); /* increment modification count */
		}
		if (DEBUG) {
			System.out.println("AbstractTracked.untrack[removed]: " + item); //$NON-NLS-1$
		}
		/* Call customizer outside of synchronized region */
		actionSet.addCustomizerRemoved(item, related, object);
		/*
		 * If the customizer throws an unchecked exception, it is safe to let it
		 * propagate
		 */
		return actionSet;
	}

	/**
	 * Returns the number of tracked items.
	 * 
	 * @return The number of tracked items.
	 * 
	 * @GuardedBy this
	 */
	int size() {
		return tracked.size();
	}

	/**
	 * Return the customized object for the specified item
	 * 
	 * @param item The item to lookup in the map
	 * @return The customized object for the specified item.
	 * 
	 * @GuardedBy this
	 */
	Object getCustomizedObject(final Object item) {
		return tracked.get(item);
	}

	/**
	 * Return the list of tracked items.
	 * 
	 * @param list An array to contain the tracked items.
	 * @return The specified list if it is large enough to hold the tracked
	 *         items or a new array large enough to hold the tracked items.
	 * @GuardedBy this
	 */
	Object[] getTracked(final Object[] list) {
		return tracked.keySet().toArray(list);
	}

	/**
	 * Increment the modification count. If this method is overridden, the
	 * overriding method MUST call this method to increment the tracking count.
	 * 
	 * @GuardedBy this
	 */
	void modified() {
		trackingCount++;
	}

	/**
	 * Returns the tracking count for this <code>ServiceTracker</code> object.
	 * 
	 * The tracking count is initialized to 0 when this object is opened. Every
	 * time an item is added, modified or removed from this object the tracking
	 * count is incremented.
	 * 
	 * @GuardedBy this
	 * @return The tracking count for this object.
	 */
	int getTrackingCount() {
		return trackingCount;
	}
	
	/**
	 * Returns the serial executor used by this tracked.
	 * @return
	 */
	SerialExecutor getExecutor() {
		return m_executor;
	}

	/**
	 * Call the specific customizer adding method. This method must not be
	 * called while synchronized on this object.
	 * 
	 * @param item Item to be tracked.
	 * @param related Action related object.
	 * @return Customized object for the tracked item or <code>null</code> if
	 *         the item is not to be tracked.
	 */
	abstract Object customizerAdding(final Object item, final Object related);

	/** marrs: Call the specific customizer added method. */
	abstract void customizerAdded(final Object item, final Object related, final Object object);
	
	/**
	 * Call the specific customizer modified method. This method must not be
	 * called while synchronized on this object.
	 * 
	 * @param item Tracked item.
	 * @param related Action related object.
	 * @param object Customized object for the tracked item.
	 */
	abstract void customizerModified(final Object item, final Object related,
			final Object object);

	/**
	 * Call the specific customizer removed method. This method must not be
	 * called while synchronized on this object.
	 * 
	 * @param item Tracked item.
	 * @param related Action related object.
	 * @param object Customized object for the tracked item.
	 */
	abstract void customizerRemoved(final Object item, final Object related,
			final Object object);
}
