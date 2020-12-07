/*
 * Copyright (C) 2013 Brett Wooldridge
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.zaxxer.hikari;

import java.io.Closeable;
import java.io.PrintWriter;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.SQLFeatureNotSupportedException;
import java.util.concurrent.atomic.AtomicBoolean;

import javax.sql.DataSource;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.zaxxer.hikari.metrics.MetricsTrackerFactory;
import com.zaxxer.hikari.pool.HikariPool;

/**
 * The HikariCP pooled DataSource.
 *
 * @author Brett Wooldridge
 */
public class HikariDataSource extends HikariConfig implements DataSource, Closeable
{
   private static final Logger LOGGER = LoggerFactory.getLogger(HikariDataSource.class);

   private final AtomicBoolean isShutdown = new AtomicBoolean();

   private final HikariPool fastPathPool;
   private volatile HikariPool pool;

   /**
    * Default constructor.  Setters be used to configure the pool.  Using
    * this constructor vs. {@link #HikariDataSource(HikariConfig)} will
    * result in {@link #getConnection()} performance that is slightly lower
    * due to lazy initialization checks.
    */
   public HikariDataSource()
   {
      super();
      fastPathPool = null;
   }

   /**
    * Construct a HikariDataSource with the specified configuration.
    *
    * @param configuration a HikariConfig instance
    */
   public HikariDataSource(HikariConfig configuration)
   {
      configuration.validate();
      configuration.copyState(this);

      LOGGER.info("{} - is starting.", configuration.getPoolName());
      pool = fastPathPool = new HikariPool(this);
   }

   /** {@inheritDoc} */
   @Override
   public Connection getConnection() throws SQLException
   {
      if (isClosed()) {
         throw new SQLException("HikariDataSource " + this + " has been closed.");
      }

      if (fastPathPool != null) {
         return fastPathPool.getConnection();
      }

      // See http://en.wikipedia.org/wiki/Double-checked_locking#Usage_in_Java
      HikariPool result = pool;
      if (result == null) {
         synchronized (this) {
            result = pool;
            if (result == null) {
               validate();
               LOGGER.info("{} - is starting.", getPoolName());
               pool = result = new HikariPool(this);
            }
         }
      }

      return result.getConnection();
   }

   /** {@inheritDoc} */
   @Override
   public Connection getConnection(String username, String password) throws SQLException
   {
      throw new SQLFeatureNotSupportedException();
   }

   /** {@inheritDoc} */
   @Override
   public PrintWriter getLogWriter() throws SQLException
   {
      return (pool != null ? pool.getUnwrappedDataSource().getLogWriter() : null);
   }

   /** {@inheritDoc} */
   @Override
   public void setLogWriter(PrintWriter out) throws SQLException
   {
      if (pool != null) {
         pool.getUnwrappedDataSource().setLogWriter(out);
      }
   }

   /** {@inheritDoc} */
   @Override
   public void setLoginTimeout(int seconds) throws SQLException
   {
      if (pool != null) {
         pool.getUnwrappedDataSource().setLoginTimeout(seconds);
      }
   }

   /** {@inheritDoc} */
   @Override
   public int getLoginTimeout() throws SQLException
   {
      return (pool != null ? pool.getUnwrappedDataSource().getLoginTimeout() : 0);
   }

   /** {@inheritDoc} */
   @Override
   public java.util.logging.Logger getParentLogger() throws SQLFeatureNotSupportedException
   {
      throw new SQLFeatureNotSupportedException();
   }

   /** {@inheritDoc} */
   @Override
   @SuppressWarnings("unchecked")
   public <T> T unwrap(Class<T> iface) throws SQLException
   {
      if (iface.isInstance(this)) {
         return (T) this;
      }

      if (pool != null) {
         if (iface.isInstance(pool.getUnwrappedDataSource())) {
            return (T) pool.getUnwrappedDataSource();
         }

         if (pool.getUnwrappedDataSource() != null) {
            return (T) pool.getUnwrappedDataSource().unwrap(iface);
         }
      }

      throw new SQLException("Wrapped DataSource is not an instance of " + iface);
   }

   /** {@inheritDoc} */
   @Override
   public boolean isWrapperFor(Class<?> iface) throws SQLException
   {
      if (iface.isInstance(this)) {
         return true;
      }
      
      if (pool != null) {
         if (iface.isInstance(pool.getUnwrappedDataSource())) {
            return true;
         }

         if (pool.getUnwrappedDataSource() != null) {
            return pool.getUnwrappedDataSource().isWrapperFor(iface);
         }
      }

      return false;
   }

   /** {@inheritDoc} */
   @Override
   public void setMetricRegistry(Object metricRegistry)
   {
      boolean isAlreadySet = getMetricRegistry() != null;
      super.setMetricRegistry(metricRegistry);

      if (pool != null) {
         if (isAlreadySet) {
            throw new IllegalStateException("MetricRegistry can only be set one time");
         }
         else {
            pool.setMetricRegistry(super.getMetricRegistry());
         }
      }
   }

   /** {@inheritDoc} */
   @Override
   public void setMetricsTrackerFactory(MetricsTrackerFactory metricsTrackerFactory)
   {
      boolean isAlreadySet = getMetricsTrackerFactory() != null;
      super.setMetricsTrackerFactory(metricsTrackerFactory);

      if (pool != null) {
         if (isAlreadySet) {
            throw new IllegalStateException("MetricsTrackerFactory can only be set one time");
         }
         else {
            pool.setMetricsTrackerFactory(super.getMetricsTrackerFactory());
         }
      }
   }

   /** {@inheritDoc} */
   @Override
   public void setHealthCheckRegistry(Object healthCheckRegistry)
   {
      boolean isAlreadySet = getHealthCheckRegistry() != null;
      super.setHealthCheckRegistry(healthCheckRegistry);

      if (pool != null) {
         if (isAlreadySet) {
            throw new IllegalStateException("HealthCheckRegistry can only be set one time");
         }
         else {
            pool.setHealthCheckRegistry(super.getHealthCheckRegistry());
         }
      }
   }

   /**
    * Evict a connection from the pool.
    *
    * @param connection the connection to evict from the pool
    */
   public void evictConnection(Connection connection)
   {
      if (!isClosed() && pool != null && connection.getClass().getName().startsWith("com.zaxxer.hikari")) {
         pool.evictConnection(connection);
      }
   }

   /**
    * Suspend allocation of connections from the pool.  All callers to <code>getConnection()</code>
    * will block indefinitely until <code>resumePool()</code> is called.
    */
   public void suspendPool()
   {
      if (!isClosed() && pool != null) {
         pool.suspendPool();
      }
   }

   /**
    * Resume allocation of connections from the pool.
    */
   public void resumePool()
   {
      if (!isClosed() && pool != null) {
         pool.resumePool();
      }
   }

   /**
    * Shutdown the DataSource and its associated pool.
    */
   @Override
   public void close()
   {
      if (isShutdown.getAndSet(true)) {
         return;
      }

      if (pool != null) {
         try {
            pool.shutdown();
         }
         catch (InterruptedException e) {
        	 LOGGER.warn("Interrupted during closing", e);
         }
      }
   }

   /**
    * Determine whether the HikariDataSource has been closed.
    *
    * @return true if the HikariDataSource has been closed, false otherwise
    */
   public boolean isClosed()
   {
      return isShutdown.get();
   }

   /**
    * Shutdown the DataSource and its associated pool.
    *
    * @deprecated This method has been deprecated, please use {@link #close()} instead
    */
   @Deprecated
   public void shutdown()
   {
      LOGGER.warn("The shutdown() method has been deprecated, please use the close() method instead");
      close();
   }

   /** {@inheritDoc} */
   @Override
   public String toString()
   {
      return "HikariDataSource (" + pool + ")";
   }
}
