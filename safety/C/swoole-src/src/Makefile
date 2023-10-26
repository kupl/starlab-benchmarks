srcdir = /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src
builddir = /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src
top_srcdir = /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src
top_builddir = /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src
EGREP = /bin/grep -E
SED = /bin/sed
CONFIGURE_COMMAND = './configure'
CONFIGURE_OPTIONS =
SHLIB_SUFFIX_NAME = so
SHLIB_DL_SUFFIX_NAME = so
AWK = gawk
SWOOLE_SHARED_LIBADD = -lz -lrt
shared_objects_swoole = swoole.lo swoole_server.lo swoole_server_port.lo swoole_atomic.lo swoole_lock.lo swoole_client.lo swoole_event.lo swoole_timer.lo swoole_async.lo swoole_process.lo swoole_buffer.lo swoole_table.lo swoole_http_server.lo swoole_http_v2_server.lo swoole_websocket_server.lo swoole_http_client.lo swoole_mysql.lo swoole_redis.lo swoole_redis_server.lo swoole_module.lo swoole_mmap.lo swoole_channel.lo src/core/base.lo src/core/log.lo src/core/hashmap.lo src/core/RingQueue.lo src/core/Channel.lo src/core/string.lo src/core/array.lo src/core/socket.lo src/core/list.lo src/core/heap.lo src/memory/ShareMemory.lo src/memory/MemoryGlobal.lo src/memory/RingBuffer.lo src/memory/FixedPool.lo src/memory/Malloc.lo src/memory/Table.lo src/memory/Buffer.lo src/factory/Factory.lo src/factory/FactoryThread.lo src/factory/FactoryProcess.lo src/reactor/ReactorBase.lo src/reactor/ReactorSelect.lo src/reactor/ReactorPoll.lo src/reactor/ReactorEpoll.lo src/reactor/ReactorKqueue.lo src/pipe/PipeBase.lo src/pipe/PipeEventfd.lo src/pipe/PipeUnsock.lo src/lock/Semaphore.lo src/lock/Mutex.lo src/lock/RWLock.lo src/lock/SpinLock.lo src/lock/FileLock.lo src/network/Server.lo src/network/TaskWorker.lo src/network/Client.lo src/network/Connection.lo src/network/ProcessPool.lo src/network/ThreadPool.lo src/network/ReactorThread.lo src/network/ReactorProcess.lo src/network/Manager.lo src/network/Worker.lo src/network/Timer.lo src/network/Port.lo src/os/base.lo src/os/dl.lo src/os/linux_aio.lo src/os/msg_queue.lo src/os/sendfile.lo src/os/signal.lo src/os/timer.lo src/protocol/Base.lo src/protocol/SSL.lo src/protocol/Http.lo src/protocol/Http2.lo src/protocol/WebSocket.lo src/protocol/Mqtt.lo src/protocol/Socks5.lo src/protocol/MimeTypes.lo src/protocol/Redis.lo src/protocol/Base64.lo thirdparty/php_http_parser.lo thirdparty/multipart_parser.lo
PHP_PECL_EXTENSION = swoole
PHP_MODULES = $(phplibdir)/swoole.la
PHP_ZEND_EX =
all_targets = $(PHP_MODULES) $(PHP_ZEND_EX)
install_targets = install-modules install-headers
prefix = /usr/local
exec_prefix = $(prefix)
libdir = ${exec_prefix}/lib
prefix = /usr/local
phplibdir = /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/modules
phpincludedir = /usr/local/include/php
CC = cc
CFLAGS = -Wall -pthread -g -O2
CFLAGS_CLEAN = $(CFLAGS)
CPP = cc -E
CPPFLAGS = -DHAVE_CONFIG_H
CXX =
CXXFLAGS =
CXXFLAGS_CLEAN = $(CXXFLAGS)
EXTENSION_DIR = /usr/local/lib/php/extensions/no-debug-non-zts-20190902
PHP_EXECUTABLE = /usr/local/bin/php
EXTRA_LDFLAGS =
EXTRA_LIBS =
INCLUDES = -I/usr/local/include/php -I/usr/local/include/php/main -I/usr/local/include/php/TSRM -I/usr/local/include/php/Zend -I/usr/local/include/php/ext -I/usr/local/include/php/ext/date/lib -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/include
LFLAGS =
LDFLAGS = -lpthread
SHARED_LIBTOOL =
LIBTOOL = $(SHELL) $(top_builddir)/libtool
SHELL = /bin/bash
INSTALL_HEADERS =
mkinstalldirs = $(top_srcdir)/build/shtool mkdir -p
INSTALL = $(top_srcdir)/build/shtool install -c
INSTALL_DATA = $(INSTALL) -m 644

DEFS = -DPHP_ATOM_INC -I$(top_builddir)/include -I$(top_builddir)/main -I$(top_srcdir)
COMMON_FLAGS = $(DEFS) $(INCLUDES) $(EXTRA_INCLUDES) $(CPPFLAGS) $(PHP_FRAMEWORKPATH)

all: $(all_targets)
	@echo
	@echo "Build complete."
	@echo "Don't forget to run 'make test'."
	@echo

build-modules: $(PHP_MODULES) $(PHP_ZEND_EX)

build-binaries: $(PHP_BINARIES)

libphp$(PHP_MAJOR_VERSION).la: $(PHP_GLOBAL_OBJS) $(PHP_SAPI_OBJS)
	$(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(EXTRA_CFLAGS) -rpath $(phptempdir) $(EXTRA_LDFLAGS) $(LDFLAGS) $(PHP_RPATHS) $(PHP_GLOBAL_OBJS) $(PHP_SAPI_OBJS) $(EXTRA_LIBS) $(ZEND_EXTRA_LIBS) -o $@
	-@$(LIBTOOL) --silent --mode=install cp $@ $(phptempdir)/$@ >/dev/null 2>&1

libs/libphp$(PHP_MAJOR_VERSION).bundle: $(PHP_GLOBAL_OBJS) $(PHP_SAPI_OBJS)
	$(CC) $(MH_BUNDLE_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) $(PHP_GLOBAL_OBJS:.lo=.o) $(PHP_SAPI_OBJS:.lo=.o) $(PHP_FRAMEWORKS) $(EXTRA_LIBS) $(ZEND_EXTRA_LIBS) -o $@ && cp $@ libs/libphp$(PHP_MAJOR_VERSION).so

install: $(all_targets) $(install_targets)

install-sapi: $(OVERALL_TARGET)
	@echo "Installing PHP SAPI module:       $(PHP_SAPI)"
	-@$(mkinstalldirs) $(INSTALL_ROOT)$(bindir)
	-@if test ! -r $(phptempdir)/libphp$(PHP_MAJOR_VERSION).$(SHLIB_DL_SUFFIX_NAME); then \
		for i in 0.0.0 0.0 0; do \
			if test -r $(phptempdir)/libphp$(PHP_MAJOR_VERSION).$(SHLIB_DL_SUFFIX_NAME).$$i; then \
				$(LN_S) $(phptempdir)/libphp$(PHP_MAJOR_VERSION).$(SHLIB_DL_SUFFIX_NAME).$$i $(phptempdir)/libphp$(PHP_MAJOR_VERSION).$(SHLIB_DL_SUFFIX_NAME); \
				break; \
			fi; \
		done; \
	fi
	@$(INSTALL_IT)

install-binaries: build-binaries $(install_binary_targets)

install-modules: build-modules
	@test -d modules && \
	$(mkinstalldirs) $(INSTALL_ROOT)$(EXTENSION_DIR)
	@echo "Installing shared extensions:     $(INSTALL_ROOT)$(EXTENSION_DIR)/"
	@rm -f modules/*.la >/dev/null 2>&1
	@$(INSTALL) modules/* $(INSTALL_ROOT)$(EXTENSION_DIR)

install-headers:
	-@if test "$(INSTALL_HEADERS)"; then \
		for i in `echo $(INSTALL_HEADERS)`; do \
			i=`$(top_srcdir)/build/shtool path -d $$i`; \
			paths="$$paths $(INSTALL_ROOT)$(phpincludedir)/$$i"; \
		done; \
		$(mkinstalldirs) $$paths && \
		echo "Installing header files:          $(INSTALL_ROOT)$(phpincludedir)/" && \
		for i in `echo $(INSTALL_HEADERS)`; do \
			if test "$(PHP_PECL_EXTENSION)"; then \
				src=`echo $$i | $(SED) -e "s#ext/$(PHP_PECL_EXTENSION)/##g"`; \
			else \
				src=$$i; \
			fi; \
			if test -f "$(top_srcdir)/$$src"; then \
				$(INSTALL_DATA) $(top_srcdir)/$$src $(INSTALL_ROOT)$(phpincludedir)/$$i; \
			elif test -f "$(top_builddir)/$$src"; then \
				$(INSTALL_DATA) $(top_builddir)/$$src $(INSTALL_ROOT)$(phpincludedir)/$$i; \
			else \
				(cd $(top_srcdir)/$$src && $(INSTALL_DATA) *.h $(INSTALL_ROOT)$(phpincludedir)/$$i; \
				cd $(top_builddir)/$$src && $(INSTALL_DATA) *.h $(INSTALL_ROOT)$(phpincludedir)/$$i) 2>/dev/null || true; \
			fi \
		done; \
	fi

PHP_TEST_SETTINGS = -d 'open_basedir=' -d 'output_buffering=0' -d 'memory_limit=-1'
PHP_TEST_SHARED_EXTENSIONS =  ` \
	if test "x$(PHP_MODULES)" != "x"; then \
		for i in $(PHP_MODULES)""; do \
			. $$i; $(top_srcdir)/build/shtool echo -n -- " -d extension=$$dlname"; \
		done; \
	fi; \
	if test "x$(PHP_ZEND_EX)" != "x"; then \
		for i in $(PHP_ZEND_EX)""; do \
			. $$i; $(top_srcdir)/build/shtool echo -n -- " -d zend_extension=$(top_builddir)/modules/$$dlname"; \
		done; \
	fi`
PHP_DEPRECATED_DIRECTIVES_REGEX = '^(magic_quotes_(gpc|runtime|sybase)?|(zend_)?extension(_debug)?(_ts)?)[\t\ ]*='

test: all
	@if test ! -z "$(PHP_EXECUTABLE)" && test -x "$(PHP_EXECUTABLE)"; then \
		INI_FILE=`$(PHP_EXECUTABLE) -d 'display_errors=stderr' -r 'echo php_ini_loaded_file();' 2> /dev/null`; \
		if test "$$INI_FILE"; then \
			$(EGREP) -h -v $(PHP_DEPRECATED_DIRECTIVES_REGEX) "$$INI_FILE" > $(top_builddir)/tmp-php.ini; \
		else \
			echo > $(top_builddir)/tmp-php.ini; \
		fi; \
		INI_SCANNED_PATH=`$(PHP_EXECUTABLE) -d 'display_errors=stderr' -r '$$a = explode(",\n", trim(php_ini_scanned_files())); echo $$a[0];' 2> /dev/null`; \
		if test "$$INI_SCANNED_PATH"; then \
			INI_SCANNED_PATH=`$(top_srcdir)/build/shtool path -d $$INI_SCANNED_PATH`; \
			$(EGREP) -h -v $(PHP_DEPRECATED_DIRECTIVES_REGEX) "$$INI_SCANNED_PATH"/*.ini >> $(top_builddir)/tmp-php.ini; \
		fi; \
		TEST_PHP_EXECUTABLE=$(PHP_EXECUTABLE) \
		TEST_PHP_SRCDIR=$(top_srcdir) \
		CC="$(CC)" \
			$(PHP_EXECUTABLE) -n -c $(top_builddir)/tmp-php.ini $(PHP_TEST_SETTINGS) $(top_srcdir)/run-tests.php -n -c $(top_builddir)/tmp-php.ini -d extension_dir=$(top_builddir)/modules/ $(PHP_TEST_SHARED_EXTENSIONS) $(TESTS); \
		TEST_RESULT_EXIT_CODE=$$?; \
		rm $(top_builddir)/tmp-php.ini; \
		exit $$TEST_RESULT_EXIT_CODE; \
	else \
		echo "ERROR: Cannot run tests without CLI sapi."; \
	fi

clean:
	find . -name \*.gcno -o -name \*.gcda | xargs rm -f
	find . -name \*.lo -o -name \*.o | xargs rm -f
	find . -name \*.la -o -name \*.a | xargs rm -f
	find . -name \*.so | xargs rm -f
	find . -name .libs -a -type d|xargs rm -rf
	rm -f libphp$(PHP_MAJOR_VERSION).la $(SAPI_CLI_PATH) $(SAPI_CGI_PATH) $(SAPI_LITESPEED_PATH) $(SAPI_FPM_PATH) $(OVERALL_TARGET) modules/* libs/*

distclean: clean
	rm -f Makefile config.cache config.log config.status Makefile.objects Makefile.fragments libtool main/php_config.h main/internal_functions_cli.c main/internal_functions.c Zend/zend_dtrace_gen.h Zend/zend_dtrace_gen.h.bak Zend/zend_config.h
	rm -f main/build-defs.h scripts/phpize
	rm -f ext/date/lib/timelib_config.h ext/mbstring/libmbfl/config.h ext/oci8/oci8_dtrace_gen.h ext/oci8/oci8_dtrace_gen.h.bak
	rm -f scripts/man1/phpize.1 scripts/php-config scripts/man1/php-config.1 sapi/cli/php.1 sapi/cgi/php-cgi.1 sapi/phpdbg/phpdbg.1 ext/phar/phar.1 ext/phar/phar.phar.1
	rm -f sapi/fpm/php-fpm.conf sapi/fpm/init.d.php-fpm sapi/fpm/php-fpm.service sapi/fpm/php-fpm.8 sapi/fpm/status.html
	rm -f ext/iconv/php_have_bsd_iconv.h ext/iconv/php_have_glibc_iconv.h ext/iconv/php_have_ibm_iconv.h ext/iconv/php_have_iconv.h ext/iconv/php_have_libiconv.h ext/iconv/php_iconv_aliased_libiconv.h ext/iconv/php_iconv_supports_errno.h ext/iconv/php_php_iconv_h_path.h ext/iconv/php_php_iconv_impl.h
	rm -f ext/phar/phar.phar ext/phar/phar.php
	if test "$(srcdir)" != "$(builddir)"; then \
	  rm -f ext/phar/phar/phar.inc; \
	fi
	$(EGREP) define'.*include/php' $(top_srcdir)/configure | $(SED) 's/.*>//'|xargs rm -f

prof-gen:
	CCACHE_DISABLE=1 $(MAKE) PROF_FLAGS=-fprofile-generate all

prof-clean:
	find . -name \*.lo -o -name \*.o | xargs rm -f
	find . -name \*.la -o -name \*.a | xargs rm -f
	find . -name \*.so | xargs rm -f
	rm -f libphp$(PHP_MAJOR_VERSION).la $(SAPI_CLI_PATH) $(SAPI_CGI_PATH) $(SAPI_LITESPEED_PATH) $(SAPI_FPM_PATH) $(OVERALL_TARGET) modules/* libs/*

prof-use:
	CCACHE_DISABLE=1 $(MAKE) PROF_FLAGS=-fprofile-use all


.PHONY: all clean install distclean test prof-gen prof-clean prof-use
.NOEXPORT:
swoole.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole.c -o swoole.lo 
swoole_server.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_server.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_server.c -o swoole_server.lo 
swoole_server_port.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_server_port.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_server_port.c -o swoole_server_port.lo 
swoole_atomic.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_atomic.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_atomic.c -o swoole_atomic.lo 
swoole_lock.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_lock.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_lock.c -o swoole_lock.lo 
swoole_client.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_client.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_client.c -o swoole_client.lo 
swoole_event.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_event.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_event.c -o swoole_event.lo 
swoole_timer.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_timer.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_timer.c -o swoole_timer.lo 
swoole_async.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_async.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_async.c -o swoole_async.lo 
swoole_process.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_process.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_process.c -o swoole_process.lo 
swoole_buffer.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_buffer.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_buffer.c -o swoole_buffer.lo 
swoole_table.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_table.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_table.c -o swoole_table.lo 
swoole_http_server.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_http_server.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_http_server.c -o swoole_http_server.lo 
swoole_http_v2_server.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_http_v2_server.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_http_v2_server.c -o swoole_http_v2_server.lo 
swoole_websocket_server.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_websocket_server.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_websocket_server.c -o swoole_websocket_server.lo 
swoole_http_client.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_http_client.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_http_client.c -o swoole_http_client.lo 
swoole_mysql.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_mysql.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_mysql.c -o swoole_mysql.lo 
swoole_redis.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_redis.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_redis.c -o swoole_redis.lo 
swoole_redis_server.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_redis_server.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_redis_server.c -o swoole_redis_server.lo 
swoole_module.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_module.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_module.c -o swoole_module.lo 
swoole_mmap.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_mmap.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_mmap.c -o swoole_mmap.lo 
swoole_channel.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_channel.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/swoole_channel.c -o swoole_channel.lo 
src/core/base.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/base.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/base.c -o src/core/base.lo 
src/core/log.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/log.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/log.c -o src/core/log.lo 
src/core/hashmap.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/hashmap.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/hashmap.c -o src/core/hashmap.lo 
src/core/RingQueue.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/RingQueue.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/RingQueue.c -o src/core/RingQueue.lo 
src/core/Channel.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/Channel.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/Channel.c -o src/core/Channel.lo 
src/core/string.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/string.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/string.c -o src/core/string.lo 
src/core/array.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/array.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/array.c -o src/core/array.lo 
src/core/socket.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/socket.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/socket.c -o src/core/socket.lo 
src/core/list.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/list.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/list.c -o src/core/list.lo 
src/core/heap.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/heap.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/core/heap.c -o src/core/heap.lo 
src/memory/ShareMemory.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/memory/ShareMemory.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/memory/ShareMemory.c -o src/memory/ShareMemory.lo 
src/memory/MemoryGlobal.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/memory/MemoryGlobal.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/memory/MemoryGlobal.c -o src/memory/MemoryGlobal.lo 
src/memory/RingBuffer.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/memory/RingBuffer.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/memory/RingBuffer.c -o src/memory/RingBuffer.lo 
src/memory/FixedPool.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/memory/FixedPool.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/memory/FixedPool.c -o src/memory/FixedPool.lo 
src/memory/Malloc.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/memory/Malloc.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/memory/Malloc.c -o src/memory/Malloc.lo 
src/memory/Table.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/memory/Table.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/memory/Table.c -o src/memory/Table.lo 
src/memory/Buffer.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/memory/Buffer.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/memory/Buffer.c -o src/memory/Buffer.lo 
src/factory/Factory.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/factory/Factory.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/factory/Factory.c -o src/factory/Factory.lo 
src/factory/FactoryThread.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/factory/FactoryThread.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/factory/FactoryThread.c -o src/factory/FactoryThread.lo 
src/factory/FactoryProcess.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/factory/FactoryProcess.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/factory/FactoryProcess.c -o src/factory/FactoryProcess.lo 
src/reactor/ReactorBase.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/reactor/ReactorBase.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/reactor/ReactorBase.c -o src/reactor/ReactorBase.lo 
src/reactor/ReactorSelect.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/reactor/ReactorSelect.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/reactor/ReactorSelect.c -o src/reactor/ReactorSelect.lo 
src/reactor/ReactorPoll.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/reactor/ReactorPoll.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/reactor/ReactorPoll.c -o src/reactor/ReactorPoll.lo 
src/reactor/ReactorEpoll.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/reactor/ReactorEpoll.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/reactor/ReactorEpoll.c -o src/reactor/ReactorEpoll.lo 
src/reactor/ReactorKqueue.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/reactor/ReactorKqueue.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/reactor/ReactorKqueue.c -o src/reactor/ReactorKqueue.lo 
src/pipe/PipeBase.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/pipe/PipeBase.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/pipe/PipeBase.c -o src/pipe/PipeBase.lo 
src/pipe/PipeEventfd.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/pipe/PipeEventfd.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/pipe/PipeEventfd.c -o src/pipe/PipeEventfd.lo 
src/pipe/PipeUnsock.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/pipe/PipeUnsock.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/pipe/PipeUnsock.c -o src/pipe/PipeUnsock.lo 
src/lock/Semaphore.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/lock/Semaphore.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/lock/Semaphore.c -o src/lock/Semaphore.lo 
src/lock/Mutex.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/lock/Mutex.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/lock/Mutex.c -o src/lock/Mutex.lo 
src/lock/RWLock.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/lock/RWLock.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/lock/RWLock.c -o src/lock/RWLock.lo 
src/lock/SpinLock.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/lock/SpinLock.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/lock/SpinLock.c -o src/lock/SpinLock.lo 
src/lock/FileLock.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/lock/FileLock.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/lock/FileLock.c -o src/lock/FileLock.lo 
src/network/Server.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/Server.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/Server.c -o src/network/Server.lo 
src/network/TaskWorker.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/TaskWorker.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/TaskWorker.c -o src/network/TaskWorker.lo 
src/network/Client.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/Client.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/Client.c -o src/network/Client.lo 
src/network/Connection.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/Connection.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/Connection.c -o src/network/Connection.lo 
src/network/ProcessPool.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/ProcessPool.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/ProcessPool.c -o src/network/ProcessPool.lo 
src/network/ThreadPool.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/ThreadPool.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/ThreadPool.c -o src/network/ThreadPool.lo 
src/network/ReactorThread.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/ReactorThread.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/ReactorThread.c -o src/network/ReactorThread.lo 
src/network/ReactorProcess.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/ReactorProcess.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/ReactorProcess.c -o src/network/ReactorProcess.lo 
src/network/Manager.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/Manager.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/Manager.c -o src/network/Manager.lo 
src/network/Worker.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/Worker.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/Worker.c -o src/network/Worker.lo 
src/network/Timer.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/Timer.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/Timer.c -o src/network/Timer.lo 
src/network/Port.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/Port.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/network/Port.c -o src/network/Port.lo 
src/os/base.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/os/base.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/os/base.c -o src/os/base.lo 
src/os/dl.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/os/dl.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/os/dl.c -o src/os/dl.lo 
src/os/linux_aio.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/os/linux_aio.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/os/linux_aio.c -o src/os/linux_aio.lo 
src/os/msg_queue.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/os/msg_queue.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/os/msg_queue.c -o src/os/msg_queue.lo 
src/os/sendfile.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/os/sendfile.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/os/sendfile.c -o src/os/sendfile.lo 
src/os/signal.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/os/signal.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/os/signal.c -o src/os/signal.lo 
src/os/timer.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/os/timer.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/os/timer.c -o src/os/timer.lo 
src/protocol/Base.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/Base.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/Base.c -o src/protocol/Base.lo 
src/protocol/SSL.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/SSL.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/SSL.c -o src/protocol/SSL.lo 
src/protocol/Http.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/Http.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/Http.c -o src/protocol/Http.lo 
src/protocol/Http2.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/Http2.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/Http2.c -o src/protocol/Http2.lo 
src/protocol/WebSocket.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/WebSocket.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/WebSocket.c -o src/protocol/WebSocket.lo 
src/protocol/Mqtt.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/Mqtt.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/Mqtt.c -o src/protocol/Mqtt.lo 
src/protocol/Socks5.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/Socks5.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/Socks5.c -o src/protocol/Socks5.lo 
src/protocol/MimeTypes.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/MimeTypes.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/MimeTypes.c -o src/protocol/MimeTypes.lo 
src/protocol/Redis.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/Redis.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/Redis.c -o src/protocol/Redis.lo 
src/protocol/Base64.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/Base64.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/src/protocol/Base64.c -o src/protocol/Base64.lo 
thirdparty/php_http_parser.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/thirdparty/php_http_parser.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/thirdparty/php_http_parser.c -o thirdparty/php_http_parser.lo 
thirdparty/multipart_parser.lo: /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/thirdparty/multipart_parser.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS)  -c /home/junhee/benchmarks/ICSE20-artifacts/table1/benchmarks/swoole-src/thirdparty/multipart_parser.c -o thirdparty/multipart_parser.lo 
$(phplibdir)/swoole.la: ./swoole.la
	$(LIBTOOL) --mode=install cp ./swoole.la $(phplibdir)

./swoole.la: $(shared_objects_swoole) $(SWOOLE_SHARED_DEPENDENCIES)
	$(LIBTOOL) --mode=link $(CC) $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) $(LDFLAGS)  -o $@ -export-dynamic -avoid-version -prefer-pic -module -rpath $(phplibdir) $(EXTRA_LDFLAGS) $(shared_objects_swoole) $(SWOOLE_SHARED_LIBADD)

