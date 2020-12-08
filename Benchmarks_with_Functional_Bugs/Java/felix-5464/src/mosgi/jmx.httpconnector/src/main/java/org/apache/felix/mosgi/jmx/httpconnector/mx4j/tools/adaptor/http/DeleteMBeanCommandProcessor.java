/* 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.    
 */
package org.apache.felix.mosgi.jmx.httpconnector.mx4j.tools.adaptor.http;

import java.io.IOException;

import javax.management.JMException;
import javax.management.MalformedObjectNameException;
import javax.management.ObjectName;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

/**
 * DeleteMBeanCommandProcessor, processes a request for unregistering an MBean
 *
 * @author    <a href="mailto:tibu@users.sourceforge.net">Carlos Quiroz</a>
 * @version   $Revision: 1.1.1.1 $
 */
public class DeleteMBeanCommandProcessor extends HttpCommandProcessorAdaptor
{

 public DeleteMBeanCommandProcessor()
 {
 }


 public Document executeRequest(HttpInputStream in)
  throws IOException, JMException
 {
  Document document = builder.newDocument();

  Element root = document.createElement("MBeanOperation");
  document.appendChild(root);
  Element operationElement = document.createElement("Operation");
  operationElement.setAttribute("operation", "delete");
  root.appendChild(operationElement);

  String objectVariable = in.getVariable("objectname");
  operationElement.setAttribute("objectname", objectVariable);
  if (objectVariable == null || objectVariable.equals(""))
  {
   operationElement.setAttribute("result", "error");
   operationElement.setAttribute("errorMsg", "Incorrect parameters in the request");
   return document;
  }
  ObjectName name = null;
  try
  {
   name = new ObjectName(objectVariable);
  }
  catch (MalformedObjectNameException e)
  {
   operationElement.setAttribute("result", "error");
   operationElement.setAttribute("errorMsg", "Malformed object name");
   return document;
  }

  if (server.isRegistered(name))
  {
   try
   {
    server.unregisterMBean(name);
    operationElement.setAttribute("result", "success");
   }
   catch (Exception e)
   {
    operationElement.setAttribute("result", "error");
    operationElement.setAttribute("errorMsg", e.getMessage());
   }
  }
  else
  {
   if (name != null)
   {
    operationElement.setAttribute("result", "error");
    operationElement.setAttribute("errorMsg", "MBean " + name + " not registered");
   }
  }
  return document;
 }

}

