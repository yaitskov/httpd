/* Copyright 2015 greenbytes GmbH (https://www.greenbytes.de)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __mod_h2__h2_conn_io__
#define __mod_h2__h2_conn_io__

struct h2_config;
struct h2_session;

/* h2_io is the basic handler of a httpd connection. It keeps two brigades,
 * one for input, one for output and works with the installed connection
 * filters.
 * The read is done via a callback function, so that input can be processed
 * directly without copying.
 */
typedef struct {
    conn_rec *connection;
    apr_bucket_brigade *output;

    int is_tls;
    apr_time_t cooldown_usecs;
    apr_int64_t warmup_size;
    
    apr_size_t write_size;
    apr_time_t last_write;
    apr_int64_t bytes_written;
    
    int buffer_output;
    char *buffer;
    apr_size_t buflen;
    apr_size_t bufsize;
    int unflushed;
} h2_conn_io;

apr_status_t h2_conn_io_init(h2_conn_io *io, conn_rec *c, 
                             const struct h2_config *cfg, 
                             apr_pool_t *pool);

int h2_conn_io_is_buffered(h2_conn_io *io);

apr_status_t h2_conn_io_write(h2_conn_io *io,
                         const char *buf,
                         size_t length);
                         
apr_status_t h2_conn_io_writeb(h2_conn_io *io, apr_bucket *b);

apr_status_t h2_conn_io_consider_flush(h2_conn_io *io);

apr_status_t h2_conn_io_pass(h2_conn_io *io);
apr_status_t h2_conn_io_flush(h2_conn_io *io);
apr_status_t h2_conn_io_close(h2_conn_io *io, void *session);

#endif /* defined(__mod_h2__h2_conn_io__) */
