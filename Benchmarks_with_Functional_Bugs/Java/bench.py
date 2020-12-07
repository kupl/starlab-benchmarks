import sys.os


repos = [Bears-121]
'''
repos = [
Bears-121,
Bears-140,
Bears-169,
Bears-17,
Bears-184,
Bears-196,
Bears-200,
Bears-230,
Bears-249,
Bears-32,
Bears-46,
Bears-56,
Bears-72,
Bears-95,
Activity-3d624a5,
caelum-stella-2d2dd9c,
caelum-stella-2ec5459,
caelum-stella-e73113f,
checkstyle-536bc20,
checkstyle-8381754,
closure-compiler-9828574,
DataflowJavaSDK-c06125d,
HikariCP-ce4ff92,
javapoet-70b38e5,
javaslang-faf9ac2,
spring-data-rest-aa28aeb,
spring-hateoas-48749e7,
truth-99b314e,
Collections-33,1
Collections-360,
Felix-4960,
Lang-304,
Lang-587,
Lang-703,
Math-988,
Pdfbox-2965,
Pdfbox-2995,
Sling-4982_1,
Sling-4982_2,
Chart-14,
Chart-15,
Chart-16,
Chart-26,
Chart-4,
Collections-39,
Felix-5464,
Lang-39,
Lang-47,
]
#Chart-25-a
#Chart-25-b
'''

for i, val in enumerate(repos):
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
  cmd = 'rm rf {}/src/.git'.format(val)
  os.system(cmd)
  
  #os.chdir(DOOP_DIR)



















