#include "NgxFile.hpp"
#include "NgxUnsetValue.hpp"
#include "NgxDebugPrint.hpp"
#include "NgxHTTP.hpp"
#include "NgxArray.hpp"

static char* ngx_http_mytest(ngx_conf_t *cf, ngx_command_t *cmd, void* conf);
static void* ngx_http_mytest_create_loc_conf(ngx_conf_t *cf);

typedef struct
{
	ngx_flag_t	my_flag;
} ngx_http_mytest_conf_t;

static ngx_http_module_t	ngx_http_mytest_module_ctx = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	ngx_http_mytest_create_loc_conf,
	NULL
};

static ngx_command_t ngx_http_mytest_commands[] = {
	{
		ngx_string("mytest"),
		NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_NOARGS,
		ngx_http_mytest,
		NGX_HTTP_LOC_CONF_OFFSET,
		0,
		NULL
	},
	{
		ngx_string("isfile"),
		NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_FLAG,
		ngx_conf_set_flag_slot,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_mytest_conf_t, my_flag),
		NULL
	},

	ngx_null_command
};

ngx_module_t ngx_http_mytest_module = {
	NGX_MODULE_V1,
	&ngx_http_mytest_module_ctx,
	ngx_http_mytest_commands,
	NGX_HTTP_MODULE,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NGX_MODULE_V1_PADDING
};

static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r)
{
//	printf("\n\n");
//	NgxBuf(r, "abcdefg");
//	printf("\n\n");

	auto flag = reinterpret_cast<ngx_http_mytest_conf_t*>(
		ngx_http_get_module_loc_conf(r, ngx_http_mytest_module));

	ngx_int_t rc = ngx_nil;

	NgxPool ngxPool(r);
	NgxHTTP ngxHTTP(r);
//	NgxDebugPrint(r).printRequest();
	NgxString ngxStrFilename;
	NgxFile ngxFile;
//	A* a = (A*)ngxPool.alloc<A>();
//	ngxPool.cleanup(a);
	if (ngxHTTP.get.isnotMethod(NGX_HTTP_GET|NGX_HTTP_HEAD))
	{
		return NGX_HTTP_NOT_ALLOWED;
	}

	rc = ngxHTTP.get.discard();
	if (rc != NGX_OK)
	{
		return rc;
	}

	if (ngxHTTP.get.argsLength() == 0)
	{
		ngxStrFilename.set("/root/Documents/NginxNote.README");

	}
	else
	{
		ngxStrFilename.set(ngxHTTP.get.args(), r);
	}

	rc = ngxFile.set(ngxStrFilename, r);
	if (rc == NGX_ERROR)
	{
		return NGX_HTTP_NOT_FOUND;
	}

	rc = ngxFile.cleanup(ngx_pool_cleanup_file, r);
	if (rc != NGX_OK)
	{
		return rc;
	}

//	NgxBuf ngxBufStr(r).fulfill(ngxStrFilename); 
//	NgxBuf ngxBufFile(r).fulfill(ngxFile);

	ngxHTTP.set.status();
	ngxHTTP.set.contentType("text/plain");

	if (!flag->my_flag)
	{
		ngxHTTP.set.contentLength(ngxStrFilename.getLength());
		rc = ngxHTTP.set.send();
		if (rc != NGX_OK)
		{
			return rc;
		}
		return ngxHTTP.set.send(ngxStrFilename);
	}
	else
	{
		ngxHTTP.set.contentLength(ngxFile.getLength()*3 + ngxStrFilename.getLength());
		rc = ngxHTTP.set.send();
		if (rc != NGX_OK)
		{
			return rc;
		}
		NgxChain ch(r);
		NgxBuf __buf(r, ngxFile);
		for (int i = 0; i<3; i++)
			ch.push_back(__buf);
		ch.push_front(NgxBuf(r, ngxStrFilename));
		return ngxHTTP.set.send(ch);
	}
};

static void* ngx_http_mytest_create_loc_conf(ngx_conf_t *cf)
{
	ngx_http_mytest_conf_t* mycf = NgxPool(cf).alloc<ngx_http_mytest_conf_t>();
	mycf->my_flag = ngx_nil;
	return mycf;
}



static char* ngx_http_mytest(ngx_conf_t *cf, ngx_command_t *cmd, void* conf)
{
	ngx_http_core_loc_conf_t *clcf;
	clcf = (ngx_http_core_loc_conf_t*)ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
	clcf->handler = ngx_http_mytest_handler;
	return NGX_CONF_OK;
};

