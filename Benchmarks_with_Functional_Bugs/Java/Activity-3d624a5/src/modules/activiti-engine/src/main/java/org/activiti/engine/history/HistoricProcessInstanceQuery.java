/* Licensed under the Apache License, Version 2.0 (the "License");
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

package org.activiti.engine.history;

import java.util.Date;
import java.util.Set;

import org.activiti.engine.query.Query;



/** Allows programmatic querying of {@link HistoricProcessInstance}s.
 *  
 * @author Tom Baeyens
 * @author Joram Barrez
 * @author Falko Menge
 */
public interface HistoricProcessInstanceQuery extends Query<HistoricProcessInstanceQuery, HistoricProcessInstance>{

  /** Only select historic process instances with the given process instance.
   * {@link ProcessInstance) ids and {@link HistoricProcessInstance} ids match. */
  HistoricProcessInstanceQuery processInstanceId(String processInstanceId);
  
  /** Only select historic process instances whose id is in the given set of ids.
   * {@link ProcessInstance) ids and {@link HistoricProcessInstance} ids match. */
  HistoricProcessInstanceQuery processInstanceIds(Set<String> processInstanceIds);

  /** Only select historic process instances for the given process definition */
  HistoricProcessInstanceQuery processDefinitionId(String processDefinitionId);
  
  /** Only select historic process instances that are defined by a process
   * definition with the given key. */
  HistoricProcessInstanceQuery processDefinitionKey(String processDefinitionKey);
  
  /** Only select historic process instances with the given business key */
  HistoricProcessInstanceQuery processInstanceBusinessKey(String processInstanceBusinessKey);
  
  /** Only select historic process instances that are completely finished. */
  HistoricProcessInstanceQuery finished();
  
  /** Only select historic process instance that are not yet finished. */
  HistoricProcessInstanceQuery unfinished();
  
  /** Only select historic process instances with variables that satisfies the provided condition */
  //HistoricProcessInstanceQuery processVariableEquals(String variableName, Object variableValue);
  
  /** Only select historic process instances that were started as of the provided date. (Date will be adjusted to reflect midnight) */
  HistoricProcessInstanceQuery startDateBy(Date date);
  
  /** Only select historic process instances that were started on the provided date. */
  HistoricProcessInstanceQuery startDateOn(Date date);
  
  /** Only select historic process instances that were finished as of the provided date. (Date will be adjusted to reflect one second before midnight) */
  HistoricProcessInstanceQuery finishDateBy(Date date);
  
  /** Only select historic process instances that were finished on provided date. */
  HistoricProcessInstanceQuery finishDateOn(Date date);
  
  /** Only select historic process instance that are started by the given user. */
  HistoricProcessInstanceQuery startedBy(String userId);
  
  /** Order by the process instance id (needs to be followed by {@link #asc()} or {@link #desc()}). */
  HistoricProcessInstanceQuery orderByProcessInstanceId();
  
  /** Order by the process definition id (needs to be followed by {@link #asc()} or {@link #desc()}). */
  HistoricProcessInstanceQuery orderByProcessDefinitionId();
  
  /** Order by the business key (needs to be followed by {@link #asc()} or {@link #desc()}). */
  HistoricProcessInstanceQuery orderByProcessInstanceBusinessKey();

  /** Order by the start time (needs to be followed by {@link #asc()} or {@link #desc()}). */
  HistoricProcessInstanceQuery orderByProcessInstanceStartTime();
  
  /** Order by the end time (needs to be followed by {@link #asc()} or {@link #desc()}). */
  HistoricProcessInstanceQuery orderByProcessInstanceEndTime();
  
  /** Order by the duration of the process instance (needs to be followed by {@link #asc()} or {@link #desc()}). */
  HistoricProcessInstanceQuery orderByProcessInstanceDuration();
  
  /** Only select historic process instances started by the given process instance.
   * {@link ProcessInstance) ids and {@link HistoricProcessInstance} ids match. */
  HistoricProcessInstanceQuery superProcessInstanceId(String superProcessInstanceId);
}
