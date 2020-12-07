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

import java.io.*;
import java.util.*;

import org.codehaus.plexus.util.*;

    boolean check() {
        File file = new File(basedir, "build.log");
        String log = FileUtils.fileRead(file);

        if (log.indexOf("Bundle 'org.apache.aries.versioning:org.apache.aries.versioning.checker:0.3.1' (defined in feature 'org.apache.sling:slingfeature-maven-plugin-test:1.0.0-SNAPSHOT') declares 'org.apache.aries.versioning.check' in the 'Export-Package' header, enlisted in the 'my-global' region, which uses 'org.objectweb.asm' package that is in the 'my-deprecated' region") < 0) {
            System.out.println( "FAILED!" );
            return false;
        }
        return true;
    }

    try {
      return check();
    }
    catch(Throwable t) {
      t.printStackTrace();
      return false;
    }

    return true;
