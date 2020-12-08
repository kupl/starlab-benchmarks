/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package org.apache.felix.ipojo.handler.eventadmin.test;

import org.apache.felix.ipojo.ComponentInstance;
import org.apache.felix.ipojo.ConfigurationException;
import org.apache.felix.ipojo.MissingHandlerException;
import org.apache.felix.ipojo.UnacceptableConfiguration;
import org.apache.felix.ipojo.handler.eventadmin.test.donut.Donut;
import org.apache.felix.ipojo.handler.eventadmin.test.donut.DonutConsumer;
import org.apache.felix.ipojo.handler.eventadmin.test.donut.DonutProvider;
import org.apache.felix.ipojo.handler.eventadmin.test.donut.EventTracker;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.osgi.framework.ServiceReference;
import org.osgi.service.event.Event;

import java.util.*;

import static junit.framework.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

/**
 * Test the good behaviour of the EventAdminHandler.
 *
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
public class TestEventAdminHandlerWithNewAttributes extends Common {

    /**
     * The number of providers to test.
     */
    private static final int NUMBER_OF_PROVIDERS = 6;
    /**
     * The number of providers using the event admin handler to test.
     */
    private static final int NUMBER_OF_EAH_PROVIDERS = 4;
    /**
     * The number of consumers to test.
     */
    private static final int NUMBER_OF_CONSUMERS = 6;
    /**
     * The number of synchronous providers to test.
     */
    private static final int NUMBER_OF_SYNCHRONOUS_PROVIDERS = 3;
    /**
     * The number of slow consumers to test.
     */
    private static final int NUMBER_OF_QUICK_CONSUMERS = 3;
    /**
     * The list of topics to test.
     */
    private static final String[] TOPICS_LIST = {"foo", "bar", "nut",
            "foo,bar", "bar,nut", "foo,nut", "foo,bar,nut"};
    /**
     * The utility class instance.
     */
    public EahTestUtils m_utils;
    /**
     * The providers' instances.
     */
    private ComponentInstance[] m_providersInstances;
    /**
     * The providers' service references.
     */
    private ServiceReference[] m_providersServices;
    /**
     * The providers' services.
     */
    private DonutProvider[] m_providers;
    /**
     * The synchronous providers' services.
     */
    private DonutProvider[] m_synchronousProviders;
    /**
     * The instances of providers that uses the event admin handler.
     */
    private ComponentInstance[] m_eahProvidersInstances;
    /**
     * The services of the providers that uses the event admin handler.
     */
    private DonutProvider[] m_eahProviders;
    /**
     * The synchronous donut event provider service.
     */
    private DonutProvider m_synchronousDonutEventProvider;
    /**
     * The consumers' instances.
     */
    private ComponentInstance[] m_consumersInstances;
    /**
     * The consumers' service references.
     */
    private ServiceReference[] m_consumersServices;
    /**
     * The consumers' services.
     */
    private DonutConsumer[] m_consumers;
    /**
     * The slow consumers' services.
     */
    private DonutConsumer[] m_quickConsumers;
    /**
     * The event tracker' instances.
     */
    private ComponentInstance m_eventTrackerInstance;
    /**
     * The event tracker' service references.
     */
    private ServiceReference m_eventTrackerService;
    /**
     * The event tracker service.
     */
    private EventTracker m_eventTracker;
    /**
     * The filtered consumers' instances.
     */
    private ComponentInstance[] m_filteredConsumersInstances;
    /**
     * The filtered consumers' service references.
     */
    private ServiceReference[] m_filteredConsumersServices;
    /**
     * The filtered consumers' services.
     */
    private DonutConsumer[] m_filteredConsumers;
    /**
     * The providers' instances with specified topics.
     */
    private ComponentInstance[] m_topicsProvidersInstances;
    /**
     * The providers' service references with specified topics.
     */
    private ServiceReference[] m_topicsProvidersServices;
    /**
     * The providers' service with specified topics.
     */
    private DonutProvider[] m_topicsProviders;
    /**
     * The provider that send donuts on the "foo" topic.
     */
    private DonutProvider m_fooProvider;
    /**
     * The provider that send donuts on the "bar" topic.
     */
    private DonutProvider m_barProvider;
    /**
     * The provider that send donuts on the "nut" topic.
     */
    private DonutProvider m_nutProvider;
    /**
     * The provider that send donuts on the "foo,bar" topics.
     */
    private DonutProvider m_fooBarProvider;
    /**
     * The provider that send donuts on the "bar,nut" topics.
     */
    private DonutProvider m_barNutProvider;
    /**
     * The provider that send donuts on the "foo,nut" topics.
     */
    private DonutProvider m_fooNutProvider;
    /**
     * The provider that send donuts on the "foo,bar,nut" topics.
     */
    private DonutProvider m_fooBarNutProvider;
    /**
     * The consumers' instances with specified topics.
     */
    private ComponentInstance[] m_topicsConsumersInstances;
    /**
     * The consumers' service references with specified topics.
     */
    private ServiceReference[] m_topicsConsumersServices;
    /**
     * The consumers' service references with specified topics.
     */
    private DonutConsumer[] m_topicsConsumers;
    /**
     * The consumer that receive donuts on the "foo" topic.
     */
    private DonutConsumer m_fooConsumer;
    /**
     * The consumer that receive donuts on the "bar" topic.
     */
    private DonutConsumer m_barConsumer;
    /**
     * The consumer that receive donuts on the "nut" topic.
     */
    private DonutConsumer m_nutConsumer;
    /**
     * The consumer that receive donuts on the "foo,bar" topics.
     */
    private DonutConsumer m_fooBarConsumer;
    /**
     * The consumer that receive donuts on the "bar,nut" topics.
     */
    private DonutConsumer m_barNutConsumer;
    /**
     * The consumer that receive donuts on the "foo,nut" topics.
     */
    private DonutConsumer m_fooNutConsumer;
    /**
     * The consumer that receive donuts on the "foo,bar,nut" topics.
     */
    private DonutConsumer m_fooBarNutConsumer;

    /**
     * Initialization before test cases.
     * <p/>
     * Create all the instances
     *
     * @throws org.apache.felix.ipojo.UnacceptableConfiguration
     *          something bad happened
     * @throws org.apache.felix.ipojo.MissingHandlerException
     *          something bad happened
     * @throws org.apache.felix.ipojo.ConfigurationException
     *          something bad happened
     */
    @Before
    public void setUp()
            throws UnacceptableConfiguration, MissingHandlerException,
            ConfigurationException {

        m_utils = new EahTestUtils(bc, ipojoHelper);
        Dictionary properties = new Hashtable();

        // All the providers
        m_providersInstances = new ComponentInstance[NUMBER_OF_PROVIDERS];
        m_providersServices = new ServiceReference[NUMBER_OF_PROVIDERS];
        m_providers = new DonutProvider[NUMBER_OF_PROVIDERS];
        m_synchronousProviders = new DonutProvider[NUMBER_OF_SYNCHRONOUS_PROVIDERS];
        m_eahProviders = new DonutProvider[NUMBER_OF_EAH_PROVIDERS];
        m_eahProvidersInstances = new ComponentInstance[NUMBER_OF_EAH_PROVIDERS];
        m_topicsProvidersInstances = new ComponentInstance[TOPICS_LIST.length];
        m_topicsProvidersServices = new ServiceReference[TOPICS_LIST.length];
        m_topicsProviders = new DonutProvider[TOPICS_LIST.length];

        // Create the (asynchronous) donut provider
        properties.put("instance.name", "asynchronous donut provider");
        m_providersInstances[0] = m_utils.getDonutProviderFactory()
                .createComponentInstance(properties);

        // Create the synchronous donut provider
        properties = new Hashtable();
        properties.put("instance.name", "synchronous donut provider");
        m_providersInstances[1] = m_utils.getSynchronousDonutProvider2Factory()
                .createComponentInstance(properties);

        // Create the (asynchronous) donut event provider
        properties = new Hashtable();
        properties.put("instance.name", "asynchronous donut event provider");
        m_providersInstances[2] = m_utils.getDonutEventProviderFactory()
                .createComponentInstance(properties);

        // Create the synchronous donut event provider
        properties = new Hashtable();
        properties.put("instance.name", "synchronous donut event provider");
        m_providersInstances[3] = m_utils
                .getSynchronousDonutEventProviderFactory()
                .createComponentInstance(properties);

        // Create the (asynchronous) event provider
        properties = new Hashtable();
        properties.put("instance.name", "asynchronous event provider");
        m_providersInstances[4] = m_utils.getEventProviderFactory()
                .createComponentInstance(properties);

        // Create the synchronous event provider
        properties = new Hashtable();
        properties.put("instance.name", "synchronous event provider");
        m_providersInstances[5] = m_utils.getSynchronousEventProviderFactory()
                .createComponentInstance(properties);

        // Get all the services references
        for (int i = 0; i < NUMBER_OF_PROVIDERS; i++) {
            m_providersServices[i] = ipojoHelper.getServiceReferenceByName(
                    DonutProvider.class.getName(),
                    m_providersInstances[i].getInstanceName());
            m_providers[i] = (DonutProvider) bc
                    .getService(m_providersServices[i]);
        }
        m_synchronousProviders[0] = m_providers[1];
        m_synchronousProviders[1] = m_providers[3];
        m_synchronousProviders[2] = m_providers[5];
        m_eahProviders[0] = m_providers[0];
        m_eahProviders[1] = m_providers[1];
        m_eahProviders[2] = m_providers[2];
        m_eahProviders[3] = m_providers[3];
        m_eahProvidersInstances[0] = m_providersInstances[0];
        m_eahProvidersInstances[1] = m_providersInstances[1];
        m_eahProvidersInstances[2] = m_providersInstances[2];
        m_eahProvidersInstances[3] = m_providersInstances[3];
        m_synchronousDonutEventProvider = m_providers[3];

        // All the consumers
        m_consumersInstances = new ComponentInstance[NUMBER_OF_CONSUMERS];
        m_consumersServices = new ServiceReference[NUMBER_OF_CONSUMERS];
        m_consumers = new DonutConsumer[NUMBER_OF_CONSUMERS];
        m_quickConsumers = new DonutConsumer[NUMBER_OF_QUICK_CONSUMERS];

        m_filteredConsumersInstances = new ComponentInstance[Donut.FLAVOURS.length];
        m_filteredConsumersServices = new ServiceReference[Donut.FLAVOURS.length];
        m_filteredConsumers = new DonutConsumer[Donut.FLAVOURS.length];
        m_topicsConsumersInstances = new ComponentInstance[TOPICS_LIST.length];
        m_topicsConsumersServices = new ServiceReference[TOPICS_LIST.length];
        m_topicsConsumers = new DonutConsumer[TOPICS_LIST.length];

        // Create the (quick) donut consumer
        properties = new Hashtable();
        properties.put("instance.name", "quick donut consumer");
        m_consumersInstances[0] = m_utils.getDonutConsumer2Factory()
                .createComponentInstance(properties);

        // Create the (quick) donut event consumer
        properties = new Hashtable();
        properties.put("instance.name", "quick donut event consumer");
        m_consumersInstances[1] = m_utils.getDonutEventConsumerFactory()
                .createComponentInstance(properties);

        // Create the (quick) event consumer
        properties = new Hashtable();
        properties.put("instance.name", "quick event consumer");
        m_consumersInstances[2] = m_utils.getEventConsumerFactory()
                .createComponentInstance(properties);

        // Create the slow donut consumer
        properties = new Hashtable();
        properties.put("slow", Boolean.TRUE);
        properties.put("instance.name", "slow donut consumer");
        m_consumersInstances[3] = m_utils.getDonutConsumer2Factory()
                .createComponentInstance(properties);

        // Create the slow donut event consumer
        properties = new Hashtable();
        properties.put("instance.name", "slow donut event consumer");
        properties.put("slow", Boolean.TRUE);
        m_consumersInstances[4] = m_utils.getDonutEventConsumerFactory()
                .createComponentInstance(properties);

        // Create the slow event consumer
        properties = new Hashtable();
        properties.put("instance.name", "slow event consumer");
        properties.put("slow", Boolean.TRUE);
        m_consumersInstances[5] = m_utils.getEventConsumerFactory()
                .createComponentInstance(properties);

        // Get all the services references
        for (int i = 0; i < NUMBER_OF_CONSUMERS; i++) {
            m_consumersServices[i] = ipojoHelper.getServiceReferenceByName(
                    DonutConsumer.class.getName(),
                    m_consumersInstances[i].getInstanceName());
            m_consumers[i] = (DonutConsumer) bc
                    .getService(m_consumersServices[i]);
        }
        m_quickConsumers[0] = m_consumers[0];
        m_quickConsumers[1] = m_consumers[1];
        m_quickConsumers[2] = m_consumers[2];

        // Create the event tracker
        properties = new Hashtable();
        properties.put("instance.name", "event tracker");
        m_eventTrackerInstance = m_utils.getEventTrackerFactory()
                .createComponentInstance(properties);
        m_eventTrackerService = ipojoHelper.getServiceReferenceByName(
                EventTracker.class.getName(), m_eventTrackerInstance
                .getInstanceName());
        m_eventTracker = (EventTracker) bc
                .getService(m_eventTrackerService);

        // Create the filtered consumer
        Dictionary filter = new Hashtable();
        for (int i = 0; i < Donut.FLAVOURS.length; i++) {
            String flavour = Donut.FLAVOURS[i];
            properties = new Hashtable();
            filter.put("donut-event-subscriber", "(flavour=" + flavour + ")");
            properties.put("instance.name", flavour + " donut consumer");
            properties.put("event.filter", filter);
            m_filteredConsumersInstances[i] = m_utils
                    .getDonutEventConsumerFactory().createComponentInstance(
                            properties);
            m_filteredConsumersServices[i] = ipojoHelper
                    .getServiceReferenceByName(DonutConsumer.class
                            .getName(), m_filteredConsumersInstances[i]
                            .getInstanceName());
            m_filteredConsumers[i] = (DonutConsumer) bc
                    .getService(m_filteredConsumersServices[i]);
        }

        // Create the providers and consumers selling and receiving donuts on
        // specific topics
        Dictionary topics = new Hashtable();
        for (int i = 0; i < TOPICS_LIST.length; i++) {
            String topicsString = TOPICS_LIST[i];
            properties = new Hashtable();
            // Create provider
            topics.put("donut-publisher", topicsString);
            properties.put("event.topics", topics);
            properties.put("instance.name", topicsString + " donut provider");
            m_topicsProvidersInstances[i] = m_utils
                    .getSynchronousDonutProviderFactory()
                    .createComponentInstance(properties);
            m_topicsProvidersServices[i] = ipojoHelper
                    .getServiceReferenceByName(DonutProvider.class
                            .getName(), m_topicsProvidersInstances[i]
                            .getInstanceName());
            m_topicsProviders[i] = (DonutProvider) bc
                    .getService(m_topicsProvidersServices[i]);

            // Create consumer
            properties = new Hashtable();
            topics.put("donut-subscriber", topicsString);
            properties.put("event.topics", topics);
            properties.put("instance.name", topicsString + " donut consumer");

            m_topicsConsumersInstances[i] = m_utils.getDonutConsumer2Factory()
                    .createComponentInstance(properties);
            m_topicsConsumersServices[i] = ipojoHelper
                    .getServiceReferenceByName(DonutConsumer.class
                            .getName(), m_topicsConsumersInstances[i]
                            .getInstanceName());
            m_topicsConsumers[i] = (DonutConsumer) bc
                    .getService(m_topicsConsumersServices[i]);
            topics.remove("donut-subscriber");
        }

        m_fooProvider = m_topicsProviders[0];
        m_barProvider = m_topicsProviders[1];
        m_nutProvider = m_topicsProviders[2];
        m_fooBarProvider = m_topicsProviders[3];
        m_barNutProvider = m_topicsProviders[4];
        m_fooNutProvider = m_topicsProviders[5];
        m_fooBarNutProvider = m_topicsProviders[6];
        m_fooConsumer = m_topicsConsumers[0];
        m_barConsumer = m_topicsConsumers[1];
        m_nutConsumer = m_topicsConsumers[2];
        m_fooBarConsumer = m_topicsConsumers[3];
        m_barNutConsumer = m_topicsConsumers[4];
        m_fooNutConsumer = m_topicsConsumers[5];
        m_fooBarNutConsumer = m_topicsConsumers[6];

    }

    /**
     * Creates a subscriber listening on a pattern topic (ending with '*').
     *
     * @throws org.apache.felix.ipojo.ConfigurationException
     *          something bad happened.
     * @throws org.apache.felix.ipojo.MissingHandlerException
     *          something bad happened.
     * @throws org.apache.felix.ipojo.UnacceptableConfiguration
     *          something bad happened.
     */
    @Test
    public void testSubscriberWithPatternTopic() throws UnacceptableConfiguration, MissingHandlerException, ConfigurationException {
        Dictionary properties = new Hashtable();
        Dictionary topics = new Hashtable();

        // Create the donut consumer instance, listening on a pattern topic
        properties.put("instance.name", "subscriber with pattern topic");
        topics.put("donut-subscriber", "a/pattern/topic/*");
        properties.put("event.topics", topics);

        ComponentInstance instance = m_utils.getDonutConsumer2Factory()
                .createComponentInstance(properties);
        instance.dispose();
    }

    /**
     * Test the event handler reliability by sending events with all kinds of
     * publisher and check they are received by all kinds of subscriber.
     */
    @Test
    public void testReliability() {

        // Flush donut list for each consumer
        for (int i = 0; i < NUMBER_OF_CONSUMERS; i++) {
            m_consumers[i].clearDonuts();
        }

        // Send a lot of donut with each provider
        List sentDonuts = new ArrayList(NUMBER_OF_PROVIDERS
                * EahTestUtils.NUMBER_OF_TESTS);
        for (int i = 0; i < NUMBER_OF_PROVIDERS; i++) {
            for (int j = 0; j < EahTestUtils.NUMBER_OF_TESTS; j++) {
                sentDonuts.add(m_providers[i].sellDonut());
            }
        }

        // Wait a respectable amount of time
        EahTestUtils.sleep(EahTestUtils.BLACK_LIST_TIME
                + EahTestUtils.A_LONG_TIME);

        // Collect all received donuts for each consumer
        for (int i = 0; i < NUMBER_OF_CONSUMERS; i++) {
            List receivedDonuts = Arrays.asList(m_consumers[i].getAllDonuts());
            assertEquals(
                    "The number of received donuts must be the same as the number of sent donuts.",
                    sentDonuts.size(), receivedDonuts.size());
            assertTrue("The receiver must have eaten all sent donuts.",
                    receivedDonuts.containsAll(sentDonuts));
        }
    }

    /**
     * Test the synchronism of event sending for the component.
     * <p/>
     * This test consists to send synchronously a big amount of donuts and to
     * check immediately if it has been received (by all quick consumers).
     */
    @Test
    public void testSynchronism() {

        // Flush donut list for quick consumers
        for (int i = 0; i < NUMBER_OF_QUICK_CONSUMERS; i++) {
            m_quickConsumers[i].clearDonuts();
        }

        // Send a lot of donuts and check they are immediately received.
        Donut sentDonut;
        Donut receivedDonut;
        for (int i = 0; i < EahTestUtils.NUMBER_OF_TESTS; i++) {
            for (int j = 0; j < NUMBER_OF_SYNCHRONOUS_PROVIDERS; j++) {
                sentDonut = m_synchronousProviders[j].sellDonut();
                for (int k = 0; k < NUMBER_OF_QUICK_CONSUMERS; k++) {
                    receivedDonut = m_quickConsumers[k].getDonut();
                    assertEquals(
                            "The donut must have been received immediately and be the be the same as the sent one.",
                            sentDonut, receivedDonut);
                }
            }
        }
    }

    /**
     * Test that the received events contains the instance name of the sender.
     */
    @Test
    public void testInstanceName() {

        // Flush event list of the event tracker
        m_eventTracker.clearEvents();

        // Send donuts and check the sender instance name
        Event receivedEvent;
        for (int i = 0; i < NUMBER_OF_EAH_PROVIDERS; i++) {
            m_eahProviders[i].sellDonut();
            receivedEvent = m_eventTracker.waitForEvent();
            assertEquals(
                    "The instance name property of the received message must be the same as the sender instance name.",
                    m_eahProvidersInstances[i].getInstanceName(), receivedEvent
                    .getProperty("publisher.instance.name"));
        }
    }

    /**
     * Test the event filtering.
     * <p/>
     * This test send donuts with different flavours. Each filtered consumer
     * must receive only a certain kind of donut. Of course, all donuts must
     * have been received too.
     */
    @Test
    public void testFilters() {

        // The sent donuts, sorted by flavour
        List[] sentDonuts = new List[Donut.FLAVOURS.length];

        // Flush donut list for each filtered consumer
        for (int i = 0; i < Donut.FLAVOURS.length; i++) {
            m_filteredConsumers[i].clearDonuts();
            sentDonuts[i] = new ArrayList(EahTestUtils.NUMBER_OF_TESTS
                    / Donut.FLAVOURS.length);
        }

        // Send donuts
        for (int j = 0; j < EahTestUtils.NUMBER_OF_TESTS; j++) {
            Donut donut = m_synchronousDonutEventProvider.sellDonut();
            sentDonuts[EahTestUtils.flavourIndex(donut.getFlavour())]
                    .add(donut);
        }

        // Check the received donuts
        for (int i = 0; i < Donut.FLAVOURS.length; i++) {
            Donut[] receivedDonuts = m_filteredConsumers[i].getAllDonuts();
            assertEquals(
                    "The number of received donuts must be the same as the number of sent donuts with the matching flavour.",
                    sentDonuts[i].size(), receivedDonuts.length);
            assertTrue(
                    "The receiver must have eaten all sent donuts matching the wanted flavour.",
                    Arrays.asList(receivedDonuts).containsAll(sentDonuts[i]));
        }

    }

    /**
     * Test the event topic.
     * <p/>
     * This test send donuts on several topics. Each consumer (who listens to
     * one or several topics) must receive donuts sent on his specifics topics.
     */
    @Test
    public void testTopics() {

        // The sent donuts, sorted by topic
        int foos = 0;
        int bars = 0;
        int nuts = 0;

        // Flush consumers
        m_fooConsumer.clearDonuts();
        m_barConsumer.clearDonuts();
        m_nutConsumer.clearDonuts();
        m_fooBarConsumer.clearDonuts();
        m_barNutConsumer.clearDonuts();
        m_fooNutConsumer.clearDonuts();
        m_fooBarNutConsumer.clearDonuts();

        // Send donuts
        for (int i = 0; i < EahTestUtils.NUMBER_OF_TESTS; i++) {
            m_fooProvider.sellDonut();
            foos++;

            m_barProvider.sellDonut();
            bars++;

            m_nutProvider.sellDonut();
            nuts++;

            m_fooBarProvider.sellDonut();
            foos++;
            bars++;

            m_barNutProvider.sellDonut();
            bars++;
            nuts++;

            m_fooNutProvider.sellDonut();
            foos++;
            nuts++;

            m_fooBarNutProvider.sellDonut();
            foos++;
            bars++;
            nuts++;
        }

        // Check received donuts
        assertEquals("The number of received donuts must be correct.", foos,
                m_fooConsumer.getAllDonuts().length);
        assertEquals("The number of received donuts must be correct.", bars,
                m_barConsumer.getAllDonuts().length);
        assertEquals("The number of received donuts must be correct.", nuts,
                m_nutConsumer.getAllDonuts().length);
        assertEquals("The number of received donuts must be correct.", foos
                + bars, m_fooBarConsumer.getAllDonuts().length);
        assertEquals("The number of received donuts must be correct.", bars
                + nuts, m_barNutConsumer.getAllDonuts().length);
        assertEquals("The number of received donuts must be correct.", foos
                + nuts, m_fooNutConsumer.getAllDonuts().length);
        assertEquals("The number of received donuts must be correct.", foos
                + bars + nuts, m_fooBarNutConsumer.getAllDonuts().length);

    }

    /**
     * Finalization after test cases.
     * <p/>
     * Release all services references and destroy instances.
     */
    @After
    public void tearDown() {
        int index;
        for (index = 0; index < NUMBER_OF_PROVIDERS; index++) {
            bc.ungetService(m_providersServices[index]);
            m_providersInstances[index].dispose();
        }
        for (index = 0; index < NUMBER_OF_CONSUMERS; index++) {
            bc.ungetService(m_consumersServices[index]);
            m_consumersInstances[index].dispose();
        }
        bc.ungetService(m_eventTrackerService);
        m_eventTrackerInstance.dispose();
        for (int i = 0; i < Donut.FLAVOURS.length; i++) {
            bc.ungetService(m_filteredConsumersServices[i]);
            m_filteredConsumersInstances[i].dispose();
        }
        for (int i = 0; i < TOPICS_LIST.length; i++) {
            bc.ungetService(m_topicsProvidersServices[i]);
            m_topicsProvidersInstances[i].dispose();
            bc.ungetService(m_topicsConsumersServices[i]);
            m_topicsConsumersInstances[i].dispose();
        }

    }

}
