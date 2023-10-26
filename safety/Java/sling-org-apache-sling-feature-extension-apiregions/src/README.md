[<img src="https://sling.apache.org/res/logos/sling.png"/>](https://sling.apache.org)

 [![Build Status](https://builds.apache.org/buildStatus/icon?job=Sling/sling-org-apache-sling-feature-extension-apiregions/master)](https://builds.apache.org/job/Sling/job/sling-org-apache-sling-feature-extension-apiregions/job/master) [![Test Status](https://img.shields.io/jenkins/t/https/builds.apache.org/job/Sling/job/sling-org-apache-sling-feature-extension-apiregions/job/master.svg)](https://builds.apache.org/job/Sling/job/sling-org-apache-sling-feature-extension-apiregions/job/master/test_results_analyzer/) [![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://www.apache.org/licenses/LICENSE-2.0) [![feature](https://sling.apache.org/badges/group-feature.svg)](https://github.com/apache/sling-aggregator/blob/master/docs/groups/feature.md)

# Apache Sling API Regions Extension for the Feature Model

This component contains extensions relating to the API Regions component.
Read the documentation about [API Regions](docs/api-regions.md) for more information.

## Feature Model Analysers

This component also contains Feature Model Analysers they are contributed through the Service Loader mechanism to the set of Analysers.

Documentation can be found here: https://github.com/apache/sling-org-apache-sling-feature-analyser . These can be run as part of the 'analyse-features' goal with the [slingfeature-maven-plugin](https://github.com/apache/sling-slingfeature-maven-plugin#analyse-features-analyse-features).  

These analysers relate to API Region definitions in Feature Models.

* `api-regions`: This analyser ensures that packages listed as exports in API-Regions sections are actually exported by a bundle that's part of the feature.

* `api-regions-dependencies`: This analyser checks that packages in API regions listed earlier in the API-Regions declaration have no dependency on API regions listed later in the list. This include `Import-Package` style dependencies and also uses-clause type dependencies. Later API regions also include packages from earlier declared API regions, but not the other way around.
  * Configuration parameters:
  * `exporting-apis`: the name of the region that provides the visible APIs.
  * `hiding-apis`: the name of the region that is 'hidden' i.e. not as visible as the exporting one. The
packages in the `exporting-api` cannot depend on any packages from this region.

* `api-regions-duplicates`: This analyser ensures that packages are only listed in one region
in a given feature. If the same package is listed in multiple regions this will be an error.

* `api-regions-check-order`: This analyser checks that regions are defined in the specified
order and that the same region is only declared once. Later regions inherit the packages
expose in earlier regions in the list, so the order is important.
  * Configuration parameters:
  * `order`: A comma separated list of the region names declaring the order in which they should be found. Not all regions declared must be present, but if they are present this
order must be obeyed.

* `api-regions-crossfeature-dups`: This analyser checks whether there are exported packages in a feature model
that does _not_ opt in to the API Regions (i.e. it does not have an API-Regions section) that overlap with exported
packages from API regions in other feature models. It can prevent against unwanted results when packages are
exported from the outside which should be exported from an API Region.
This analyser only provides a useful result when run on
an aggregate feature model, i.e. a feature model that was created by aggregating a number of other feature models. It uses the
`feature-origins` metadata to find the features that bundles were initially declared in. It then matches this with the `feature-origins` found in the `api-regions` section. Exports from  bundles from features that don't
declare `api-regions` are compared to declared exports in the `api-regions` section. If there is overlap an error
is reported.
  * Configuration parameters:
  * `regions`: a comma separated list of regions to check. If not specified all regions found are checked. This configuration item can be used to exclude certain regions from the check.
  * `warningPackages`: if packages listed here are found to overlap, a warning instead of an error is reported. Supports either literal package names (e.g. `javax.servlet`) or wildcards with an asterisk at the end (e.g. `javax.*`).
  * `ignoredPackages`: packages listed here are completely ignored in the analysis. Supports literal package names or wildcards with an asterisk at the end.

## Extensions

The following extensions are registered via the ServiceLoader mechanism:

## `org.apache.sling.feature.builder.MergeHandler`
Merge handlers are called when features are merged during the aggregation process.

`APIRegionMergeHandler` - This handler knows how to merge API Regions extensions


# Additional Extensions

The following extensions are also implemented by this component and made available through the Service Loader mechanism:

* org.apache.sling.feature.launcher.spi.extensions.ExtensionHandler
* org.apache.sling.feature.launcher.spi.Launcher
* org.apache.sling.feature.scanner.spi.ExtensionScanner
