/*
 * Copyright (C) 2013 Tatsuhiko Kubo <cubicdaiya@gmail.com>
 */

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#define HTML_HELLO_WORLD                        \
    "<!DOCTYPE html>\n"                         \
    "<html>\n"                                  \
    "<head>\n"                                  \
    "<title>Hello, World with nginx!</title>\n" \
    "</head>\n"                                 \
    "<body>\n"                                  \
    "<p>Hello, World!</p>"                      \
    "</body>\n"                                 \
    "</html>\n"

static char *ngx_http_hello_world(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_hello_world_handler(ngx_http_request_t *r);

static ngx_command_t ngx_http_hello_world_commands[] = {
    { 
        ngx_string("hello_world"),
        NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
        ngx_http_hello_world,
        0,
        0,
        NULL
    },
    ngx_null_command
};


static ngx_http_module_t ngx_http_hello_world_module_ctx = {
    NULL,                              /* preconfiguration */
    NULL,                              /* postconfiguration */
    
    NULL,                              /* create main configuration */
    NULL,                              /* init main configuration */

    NULL,                              /* create server configuration */
    NULL,                              /* merge server configuration */

    NULL,                              /* create location configuration */
    NULL                               /* merge location configuration */
};

ngx_module_t ngx_http_hello_world_module = {
    NGX_MODULE_V1,
    &ngx_http_hello_world_module_ctx, /* module context */
    ngx_http_hello_world_commands,    /* module directives */
    NGX_HTTP_MODULE,                  /* module type */
    NULL,                             /* init master */
    NULL,                             /* init module */
    NULL,                             /* init process */
    NULL,                             /* init thread */
    NULL,                             /* exit thread */
    NULL,                             /* exit process */
    NULL,                             /* exit master */
    NGX_MODULE_V1_PADDING
};

static ngx_int_t ngx_http_hello_world_handler(ngx_http_request_t *r)
{
    ngx_int_t                    rc;
    ngx_chain_t                  out;
    ngx_buf_t                   *b;
    u_char                      *buf;
    ngx_uint_t                   clen;

    if (r->method != NGX_HTTP_GET) {
        return NGX_HTTP_NOT_ALLOWED;
    }

    if (r->headers_in.if_modified_since) {
        return NGX_HTTP_NOT_MODIFIED;
    }

    buf = ngx_pcalloc(r->pool, BUFSIZ + 1);
    if (buf == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    ngx_snprintf(buf, BUFSIZ, HTML_HELLO_WORLD);

    clen = ngx_strlen(buf); 

    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    if (b == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    b->pos      = buf;
    b->last     = buf + clen;
    b->memory   = 1;
    b->last_buf = 1;
    out.buf     = b;
    out.next    = NULL;

    r->headers_out.content_type.len  = ngx_strlen("text/html");
    r->headers_out.content_type.data = (u_char *) "text/html";
    r->headers_out.status            = NGX_HTTP_OK;
    r->headers_out.content_length_n  = clen;

    rc = ngx_http_send_header(r);
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        return rc;
    }

    return ngx_http_output_filter(r, &out);
}
 
static char *ngx_http_hello_world(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t  *clcf;

    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_hello_world_handler;

    return NGX_CONF_OK;
}
