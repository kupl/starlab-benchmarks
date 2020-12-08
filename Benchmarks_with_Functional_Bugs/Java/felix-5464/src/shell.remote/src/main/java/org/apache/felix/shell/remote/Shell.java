/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.felix.shell.remote;

import java.io.BufferedReader;
import java.io.IOException;
import java.net.Socket;

import org.apache.felix.shell.ShellService;
import org.apache.felix.service.command.CommandProcessor;
import org.apache.felix.service.command.CommandSession;

/**
 * Implements the shell.
 * <p>
 * This class is instantiated by the {@link Listener} thread to handle a single
 * remote connection in its own thread. The connection handler thread either
 * terminates on request by the remote end or by the Remote Shell bundle being
 * stopped. In the latter case, the {@link #terminate()} method is called, which
 * closes the Socket used to handle the remote console. This causes a
 * <code>SocketException</code> in the handler thread reading from the socket
 * which in turn causes the {@link #run()} method to terminate and thus to
 * end the handler thread. 
 */
class Shell implements Runnable
{
    private final Listener m_owner;
    private final Socket m_socket;
    private final AtomicInteger m_useCounter;
    private volatile TerminalPrintStream m_out;

    public Shell(Listener owner, Socket s, AtomicInteger counter)
    {
        m_owner = owner;
        m_socket = s;
        m_useCounter = counter;
    }//constructor

    void terminate()
    {
        // called by Listener.deactivate() to terminate this session
        exit("\r\nFelix Remote Shell Console Terminating");
    }//terminate

    /**
     * Runs the shell.
     */
    public void run()
    {
        m_owner.registerConnection(this);

        String msg = null;

        try
        {
            m_out = new TerminalPrintStream(
                m_owner.getServices(), m_socket.getOutputStream());

            Object obj = null;

            if ((obj = m_owner.getServices().getCommandProcessor(ServiceMediator.NO_WAIT))
                != null)
            {
                CommandProcessor cp = (CommandProcessor) obj;
                CommandSession session =
                    cp.createSession(m_socket.getInputStream(), m_out, m_out);
                startGogoShell(session);
            }
            else if ((obj = m_owner.getServices().getShellService(ServiceMediator.NO_WAIT))
                != null)
            {
                startFelixShell();
            }
            else
            {
                msg = "No shell services available...exiting.";
            }
        }
        catch (IOException ex)
        {
            m_owner.getServices().error("Shell::run()", ex);
        }
        finally
        {
            // no need to clean up in/out, since exit does it all
            exit(msg);
        }
    }//run

    private void startGogoShell(CommandSession session)
    {
        try
        {
            session.execute("gosh --login --noshutdown");
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
        finally
        {
            session.close();
        }
    }

    private void startFelixShell() throws IOException
    {
        BufferedReader in = new BufferedReader(
            new TerminalReader(m_socket.getInputStream(), m_out));
        ReentrantLock lock = new ReentrantLock();

        // Print welcome banner.
        m_out.println();
        m_out.println("Felix Remote Shell Console:");
        m_out.println("============================");
        m_out.println("");

        do
        {
            String line = "";
            try
            {
                m_out.print("-> ");
                line = in.readLine();
                //make sure to capture end of stream
                if (line == null)
                {
                    m_out.println("exit");
                    return;
                }
            }
            catch (Exception ex)
            {
                return;
            }

            line = line.trim();
            if (line.equalsIgnoreCase("exit") || line.equalsIgnoreCase("disconnect"))
            {
                return;
            }

            ShellService shs = (ShellService)
                m_owner.getServices().getShellService(ServiceMediator.NO_WAIT);
            try
            {
                lock.acquire();
                shs.executeCommand(line, m_out, m_out);
            }
            catch (Exception ex)
            {
                m_owner.getServices().error("Shell::run()", ex);
            }
            finally
            {
                lock.release();
            }
        }
        while (true);
    }

    private void exit(String message)
    {
        // farewell message
        if (message != null)
        {
            m_out.println(message);
        }
        m_out.println("Good Bye!");
        m_out.close();

        try
        {
            m_socket.close();
        }
        catch (IOException ex)
        {
            m_owner.getServices().error("Shell::exit()", ex);
        }
        m_owner.unregisterConnection(this);
        m_useCounter.decrement();
    }//exit
}//class Shell
