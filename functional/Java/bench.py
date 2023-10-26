import sys, os


repos = [
'spring-data-rest-aa28aeb',
#'collections-39',
]
#'spring-data-rest-aa28aeb',

#'Chart-15',
for i, val in enumerate(repos):
  print ''
  print 'Processing {}'.format(val)
  #branch_name =  val + :'-buggy'
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
  print ''
  #os.chdir(DOOP_DIR)



















