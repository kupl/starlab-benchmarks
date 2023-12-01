[<img src="https://sling.apache.org/res/logos/sling.png"/>](https://sling.apache.org)

 [![Build Status](https://builds.apache.org/buildStatus/icon?job=Sling/sling-org-apache-sling-auth-core/master)](https://builds.apache.org/job/Sling/job/sling-org-apache-sling-auth-core/job/master) [![Test Status](https://img.shields.io/jenkins/t/https/builds.apache.org/job/Sling/job/sling-org-apache-sling-auth-core/job/master.svg)](https://builds.apache.org/job/Sling/job/sling-org-apache-sling-auth-core/job/master/test_results_analyzer/) [![Maven Central](https://maven-badges.herokuapp.com/maven-central/org.apache.sling/org.apache.sling.auth.core/badge.svg)](https://search.maven.org/#search%7Cga%7C1%7Cg%3A%22org.apache.sling%22%20a%3A%22org.apache.sling.auth.core%22) [![JavaDocs](https://www.javadoc.io/badge/org.apache.sling/org.apache.sling.auth.core.svg)](https://www.javadoc.io/doc/org.apache.sling/org.apache.sling.auth.core) [![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://www.apache.org/licenses/LICENSE-2.0) [![auth](https://sling.apache.org/badges/group-auth.svg)](https://github.com/apache/sling-aggregator/blob/master/docs/groups/auth.md)

# Apache Sling Authentication Service

This module is part of the [Apache Sling](https://sling.apache.org) project.

The Sling Authentication Service bundle provides the basic mechanisms to authenticate HTTP requests with a JCR repository. The algorithms for extracting authentication details from the requests is extensible by implementing an AuthenticationHandler interface.
