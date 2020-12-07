# Any23 Web Server

This is the root dir of the Any23 Web-Server module.

Apache Any23 provides a Web-Service that can be used to extract RDF from Web documents.

## Generate Web-Server Packaging

Execute 'mvn package' from this directory.

```
$ mvn package
```
From this directory it generates roughly the following...
```
.
├── pom.xml
├── README.txt
├── src
│   ├── main
│   │   ├── assembly
│   │   ├── bin
│   │   ├── java
│   │   ├── resources
│   │   └── webapp
│   └── test
│       ├── java
│       └── resources
└── target
    ├── any23-service-${version}.war
    ├── any23-service-${version}-without-deps.war
    ├── apache-any23-service-${version}-bin-server-embedded.tar.gz <<<
    ├── apache-any23-service-${version}-bin-server-embedded.zip <<<
    ├── apache-any23-service-${version}-bin.tar.gz <<<
    ├── apache-any23-service-${version}-bin-without-deps.tar.gz <<<
    ├── apache-any23-service-${version}-bin-without-deps.zip <<<
    ├── apache-any23-service-${version}-bin.zip <<<
    ├── archive-tmp
    ├── classes
    ├── generated-sources
    ├── maven-archiver
    ├── maven-shared-archive-resources
    ├── surefire
    ├── surefire-reports
    └── test-classes
```

Specific README's for each of the artifacts can be found in either ./target/*.tar.gz || ./target/*.zip (annotated above with '<<<'), where much more detailed information sources can be located.

# Docker
It may be more convenient if you use [Docker](https://www.docker.com/) for packaging and running the Any23 server.

## Build the image
```
$ docker build -t tomcat .
```

## Run the image
```
$ docker run -d -p 8080:8080 --name tomcat tomcat
```

## Hot deploy Any23 webapp (war)
```
$ docker cp target/apache-any23-service-2.4-SNAPSHOT.war tomcat:/usr/local/tomcat/webapps/apache-any23-service-2.4-SNAPSHOT.war
```
