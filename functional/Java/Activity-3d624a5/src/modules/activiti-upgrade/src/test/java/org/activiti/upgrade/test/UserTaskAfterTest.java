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

package org.activiti.upgrade.test;

import java.util.HashMap;
import java.util.Map;

import org.activiti.engine.task.Task;


/**
 * @author Tom Baeyens
 */
public class UserTaskAfterTest extends UpgradeTestCase {

  public void testSimplestTask() {
    Task task = taskService
      .createTaskQuery()
      .taskName("simpleTask2")
      .singleResult();
    
    String processInstanceId = task.getProcessInstanceId();
    
    long expectedHistoryTaskInstances = -1;
    String schemaHistory = managementService.getProperties().get("schema.history");
    if (schemaHistory.startsWith("create(5.0)")) {
      expectedHistoryTaskInstances = 0;
    } else {
      expectedHistoryTaskInstances = 2;
    }
    
    assertEquals(expectedHistoryTaskInstances, 
      historyService.createHistoricTaskInstanceQuery()
        .processInstanceId(processInstanceId)
        .orderByTaskName().asc()
        .count());
      
    taskService.complete(task.getId());
    
    assertEquals(1, runtimeService
            .createExecutionQuery()
            .processInstanceId(processInstanceId)
            .list()
            .size());

    assertEquals(expectedHistoryTaskInstances+1, 
            historyService.createHistoricTaskInstanceQuery()
              .processInstanceId(processInstanceId)
              .orderByTaskName().asc()
              .count());
            
    task = taskService
      .createTaskQuery()
      .taskName("simpleTask3")
      .singleResult();

    taskService.complete(task.getId());

    assertEquals(0, runtimeService
            .createExecutionQuery()
            .processInstanceId(processInstanceId)
            .list()
            .size());

    assertEquals(expectedHistoryTaskInstances+1, 
            historyService.createHistoricTaskInstanceQuery()
              .processInstanceId(processInstanceId)
              .orderByTaskName().asc()
              .count());
  }

  public void testTaskWithExecutionVariables() {
    Task task = taskService
      .createTaskQuery()
      .taskName("taskWithExecutionVariables")
      .singleResult();

    String taskId = task.getId();

    assertNotNull(task);

    String processInstanceId = task.getProcessInstanceId();
    
    Map<String, Object> expectedVariables = new HashMap<String, Object>();
    assertEquals(expectedVariables, taskService.getVariablesLocal(taskId));

    expectedVariables.put("instrument", "trumpet");
    expectedVariables.put("player", "gonzo");

    assertEquals(expectedVariables, taskService.getVariables(taskId));

    taskService.complete(taskId);

    assertEquals(0, runtimeService
      .createExecutionQuery()
      .processInstanceId(processInstanceId)
      .list()
      .size());
  }
}
