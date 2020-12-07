import os, sys


#repos = ['Bears-140']
repos = [
#'spring-data-rest-aa28aeb'
#'Bears-121',
#'Bears-140',
#'Bears-169',
#'Bears-17',
#'Bears-184',
#'Bears-196',
#'Bears-200',
#'Bears-230',
#'Bears-249',
#'Bears-32',
#'Bears-46',
#'Bears-56',
#'Bears-72',
#'Bears-95',

#'math-988a',
#'Chart-25'

'sling-4982',


#'collections-331',
#'collections-360',
#'felix-4960',
#'lang-304',
#'lang-587',
#'lang-703',
#'pdfbox-2965',
#'pdfbox-2995',


#'Activity-3d624a5',
#'caelum-stella-2d2dd9c',
#'caelum-stella-2ec5459',
#'caelum-stella-e73113f',
#'checkstyle-536bc20',
#'checkstyle-8381754',
#'closure-compiler-9828574',
#'DataflowJavaSDK-c06125d',
#'HikariCP-ce4ff92',
#'javapoet-70b38e5',
#'javaslang-faf9ac2',
#'truth-99b314e',
#'Chart-14',
#'Chart-26',
#'Lang-39',
]

#'Chart-25-a'
#'Chart-25-b'
#'Sling-4982_1',
#'Sling-4982_2',

#'spring-data-rest-aa28aeb',
#'spring-hateoas-48749e7',
#'collections-39',
#'Lang-47',
#'Chart-15',
#'Chart-16',
#'Chart-4',
#'felix-5464',

for i, val in enumerate(repos):
  print 'Processing {}'.format(val)
  #branch_name =  val + '-buggy'
  branch_name = 'benchmarks/'+ val +'-buggy'
  cmd = 'git clone -b {} --single-branch https://github.com/kupl/NPEX_Data.git'.format(branch_name)
  os.system(cmd)
  cmd = 'mkdir {}'.format(val)
  os.system(cmd)
  cmd = 'mkdir {}/src'.format(val)
  os.system(cmd)
  cmd = 'mkdir {}/bug'.format(val)
  os.system(cmd)
  cmd = 'mv NPEX_Data/npe.json {}/bug/'.format(val)
  os.system(cmd)
  cmd = 'mv NPEX_Data/* {}/src/'.format(val)
  os.system(cmd)
  cmd = 'rm {}/src/*.json'.format(val)
  os.system(cmd)
  cmd = 'rm -rf {}/src/.git'.format(val)
  os.system(cmd)
  cmd = 'rm -rf NPEX_Data'
  os.system(cmd)
  #os.chdir(DOOP_DIR)



















