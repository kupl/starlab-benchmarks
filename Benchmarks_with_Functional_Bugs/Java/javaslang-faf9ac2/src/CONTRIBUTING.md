## Coding Conventions

### Javadoc

* Public API needs javadoc, e.g. public classes and public methods.
* Non-trivial private methods need javadoc, too.
* A package, which is part of the public API, contains a `package-info.java`.
* Unit tests contain no javadoc at all (because they introduce no new API and contain no business logic).
* Running `mvn javadoc:javadoc` results in no javadoc errors.
* A (non-test) class starts with
```java
/**                       ___ __          ,                   ___                                
 *  __ ___ _____  _______/  /  / ______  / \_   ______ ______/__/_____  ______  _______ _____    
 * /  '__/'  _  \/   ___/      \/   "__\/  _/__/ ____/'  ___/  /   "__\/   ,  \/   ___/'  "__\   
 * \__/  \______/\______\__/___/\______/\___/\_____/ \______\_/\______/\__/___/\______\______/.io
 * Licensed under the Apache License, Version 2.0. Copyright 2014 Daniel Dietrich.
 */
```

### Packages

* There is only one first-level package: javaslang.
* The maximum package depth is two.
* Package names are denoted in singular.
* Packages are sliced by domain.
* Util packages do not exist.
* Cyclic dependencies between packages do not exist.
* To avoid cycles, coherent classes are grouped in packages. E.g. Arrays is located in javaslang.lang instead of javaslang.collection.
* Package private classes are welcome in order to hide non-public API.
* Inner classes are preferred over package private classes in case of one-to-one dependencies.

### Exceptions

* Unchecked exceptions, i.e. RuntimeExceptions, which are not expected to be handled explicitly by the User of an API, are not declared in the method signature. Instead, unchecked exceptions are declared in the javadoc to document the API.

### Extension Methods

* Classes containing extensions methods have the name of the extended class and end with an 's'. E.g. `java.util.List` is extended by `javaslang.collection.Lists`.
* Classes containing extensions methods are final and have a private constructor.
* Extension methods are static.
* The private constructor of class Xxx is:
```java
/**
 * This class is not intendet to be instantiated.
 */
private Xxx() {
	throw new AssertionError(Xxx.class.getName() + " cannot be instantiated.");
}
```
* The extended type is the first parameter of an extension method. E.g. `Lists.lastElement(list)`.

### Class instantiation

* Classes are instantiated via `new`.
* Interface may provide static factory methods. E.g. javaslang.option.Option.of(Object obj).
* Extension methods may instantiate classes, too. 

### Unit tests

* Public API is tested.
* High-level functionality is tested in first place.
* Corner cases are tested.
* Trivial methods are not tested, e.g. getters, setters.
* The test method name documents

### 3rd party libraries

* Javaslang has no dependencies other than Java.
* Unit tests depend solely on junit and fest-assert-core.

## SCM

* Commits are coarsely granular grouped by feature/change.
* Commits do not mix change sets of different domains/purpose.
* Commit messages provide enough detail to extract a changelog for a new release.
