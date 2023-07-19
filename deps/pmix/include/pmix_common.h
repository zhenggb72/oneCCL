/* include/pmix_common.h.  Generated from pmix_common.h.in by configure.  */
/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013-2020 Intel, Inc.  All rights reserved.
 * Copyright (c) 2016-2019 Research Organization for Information Science
 *                         and Technology (RIST).  All rights reserved.
 * Copyright (c) 2016-2022 IBM Corporation.  All rights reserved.
 * Copyright (c) 2016-2019 Mellanox Technologies, Inc.
 *                         All rights reserved.
 * Copyright (c) 2021      Triad National Security, LLC. All rights
 *                         reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer listed
 *   in this license in the documentation and/or other materials
 *   provided with the distribution.
 *
 * - Neither the name of the copyright holders nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * The copyright holders provide no reassurances that the source code
 * provided does not infringe any patent, copyright, or any other
 * intellectual property rights of third parties.  The copyright holders
 * disclaim any liability to any recipient for claims brought against
 * recipient by any third party for infringement of that parties
 * intellectual property rights.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Copyright (c) 2020      Cisco Systems, Inc.  All rights reserved
 * Copyright (c) 2021-2022 Nanook Consulting  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#ifndef PMIx_COMMON_H
#define PMIx_COMMON_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/time.h> /* for struct timeval */
#include <unistd.h> /* for uid_t and gid_t */
#include <sys/types.h> /* for uid_t and gid_t */

extern char **environ;

/* Whether C compiler supports -fvisibility */
#define PMIX_HAVE_VISIBILITY 1

#if PMIX_HAVE_VISIBILITY == 1
#define PMIX_EXPORT __attribute__((__visibility__("default")))
#else
#define PMIX_EXPORT
#endif


#include <pmix_version.h>

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/****  PMIX CONSTANTS    ****/

/* define maximum value and key sizes */
#define PMIX_MAX_NSLEN     255
#define PMIX_MAX_KEYLEN    511

/* define abstract types for namespaces and keys */
typedef char pmix_nspace_t[PMIX_MAX_NSLEN+1];
typedef char pmix_key_t[PMIX_MAX_KEYLEN+1];

/* define a type for rank values */
typedef uint32_t pmix_rank_t;

/* define a value for requests for job-level data
 * where the info itself isn't associated with any
 * specific rank, or when a request involves
 * a rank that isn't known - e.g., when someone requests
 * info thru one of the legacy interfaces where the rank
 * is typically encoded into the key itself since there is
 * no rank parameter in the API itself */
#define PMIX_RANK_UNDEF     UINT32_MAX
/* define a value to indicate that the user wants the
 * data for the given key from every rank that posted
 * that key */
#define PMIX_RANK_WILDCARD  UINT32_MAX-1
/* other special rank values will be used to define
 * groups of ranks for use in collectives */
#define PMIX_RANK_LOCAL_NODE    UINT32_MAX-2        // all ranks on local node
#define PMIX_RANK_LOCAL_PEERS   UINT32_MAX-4        // all peers (i.e., all procs within the same nspace) on local node
/* define an invalid value */
#define PMIX_RANK_INVALID   UINT32_MAX-3
/* define a boundary for valid ranks */
#define PMIX_RANK_VALID         UINT32_MAX-50
/* define a macro for testing for valid ranks */
#define PMIX_RANK_IS_VALID(r)   \
    ((r) < PMIX_RANK_VALID)

/* define a value to indicate that data applies
 * to all apps in a job */
#define PMIX_APP_WILDCARD  UINT32_MAX

/****  PMIX ENVIRONMENTAL PARAMETERS  ****/
/* URI of tool waiting for launcher to rendezvous back to it */
#define PMIX_LAUNCHER_RNDZ_URI "PMIX_LAUNCHER_RNDZ_URI"

/* PMIX_LAUNCHER_RNDZ_FILE - if set, contains the full pathname
 * of a file the launcher is to write that contains its connection info.
 * Works in addition to anything else the launcher may output.
 */
#define PMIX_LAUNCHER_RNDZ_FILE "PMIX_LAUNCHER_RNDZ_FILE"

/* pipe to be monitored that indicates when the parent process
 * terminates - used by fork'd tools to identify when the tool
 * that started them has died */
#define PMIX_KEEPALIVE_PIPE "PMIX_KEEPALIVE_PIPE"


/* define a set of "standard" PMIx attributes that can
 * be queried. Implementations (and users) are free to extend as
 * desired, so the get functions need to be capable
 * of handling the "not found" condition. Note that these
 * are attributes of the system and the job as opposed to
 * values the application (or underlying MPI library)
 * might choose to expose - i.e., they are values provided
 * by the resource manager as opposed to the application. Thus,
 * these keys are RESERVED */
#define PMIX_ATTR_UNDEF      "pmix.undef"

/* initialization attributes */
#define PMIX_EXTERNAL_PROGRESS              "pmix.evext"            // (bool) The host shall progress the PMIx library via
                                                                    //        calls to PMIx_Progress
#define PMIX_SERVER_TOOL_SUPPORT            "pmix.srvr.tool"        // (bool) The host RM wants to declare itself as willing
                                                                    //        to accept tool connection requests
#define PMIX_SERVER_REMOTE_CONNECTIONS      "pmix.srvr.remote"      // (bool) Allow connections from remote tools (do not use
                                                                    //        loopback device)
#define PMIX_SERVER_SYSTEM_SUPPORT          "pmix.srvr.sys"         // (bool) The host RM wants to declare itself as being
                                                                    //        the local system server for PMIx connection
                                                                    //        requests
#define PMIX_SERVER_SESSION_SUPPORT         "pmix.srvr.sess"        // (bool) The host RM wants to declare itself as being
                                                                    //        the local session server for PMIx connection
                                                                    //        requests
#define PMIX_SERVER_TMPDIR                  "pmix.srvr.tmpdir"      // (char*) temp directory where PMIx server will place
                                                                    //        client rendezvous points and contact info
#define PMIX_SYSTEM_TMPDIR                  "pmix.sys.tmpdir"       // (char*) temp directory for this system, where PMIx
                                                                    //        server will place tool rendezvous points and
                                                                    //        contact info
#define PMIX_SERVER_SHARE_TOPOLOGY          "pmix.srvr.share"       // (bool) server is to share its copy of the local node
                                                                    // topology (whether given to it or self-discovered) with any clients.
#define PMIX_SERVER_ENABLE_MONITORING       "pmix.srv.monitor"      // (bool) Enable PMIx internal monitoring by server
#define PMIX_SERVER_NSPACE                  "pmix.srv.nspace"       // (char*) Name of the nspace to use for this server
#define PMIX_SERVER_RANK                    "pmix.srv.rank"         // (pmix_rank_t) Rank of this server
#define PMIX_SERVER_GATEWAY                 "pmix.srv.gway"         // (bool) Server is acting as a gateway for PMIx requests
                                                                    //        that cannot be serviced on backend nodes
                                                                    //        (e.g., logging to email)
#define PMIX_SERVER_SCHEDULER               "pmix.srv.sched"        // (bool) Server supports system scheduler
#define PMIX_SERVER_START_TIME              "pmix.srv.strtime"      // (char*) Time when the server started - i.e., when the server created
                                                                    //         it's rendezvous file (given in ctime string format)
#define PMIX_HOMOGENEOUS_SYSTEM             "pmix.homo"             // (bool) The nodes comprising the session are homogeneous - i.e., they
                                                                    //        each contain the same number of identical packages, fabric interfaces,
                                                                    //        GPU, and other devices
#define PMIX_SINGLETON                      "pmix.singleton"        // (char*) String representation (nspace.rank) of proc ID for the singleton
                                                                    //         the server was started to support
#define PMIX_BIND_PROGRESS_THREAD           "pmix.bind.pt"          // (char*) Comma-delimited ranges of CPUs that the internal PMIx progress
                                                                    //         thread shall be bound to
#define PMIX_BIND_REQUIRED                  "pmix.bind.reqd"        // (bool) Return error if the internal PMIx progress thread cannot be bound


/* tool-related attributes */
#define PMIX_TOOL_NSPACE                    "pmix.tool.nspace"      // (char*) Name of the nspace to use for this tool
#define PMIX_TOOL_RANK                      "pmix.tool.rank"        // (uint32_t) Rank of this tool
#define PMIX_SERVER_PIDINFO                 "pmix.srvr.pidinfo"     // (pid_t) pid of the target server for a tool
#define PMIX_CONNECT_TO_SYSTEM              "pmix.cnct.sys"         // (bool) The requestor requires that a connection be made only to
                                                                    //        a local system-level PMIx server
#define PMIX_CONNECT_SYSTEM_FIRST           "pmix.cnct.sys.first"   // (bool) Preferentially look for a system-level PMIx server first
#define PMIX_SERVER_URI                     "pmix.srvr.uri"         // (char*) URI of server to be contacted
#define PMIX_MYSERVER_URI                   "pmix.mysrvr.uri"       // (char*) URI of this proc's listener socket
#define PMIX_SERVER_HOSTNAME                "pmix.srvr.host"        // (char*) node where target server is located
#define PMIX_CONNECT_MAX_RETRIES            "pmix.tool.mretries"    // (uint32_t) maximum number of times to try to connect to server
#define PMIX_CONNECT_RETRY_DELAY            "pmix.tool.retry"       // (uint32_t) time in seconds between connection attempts
#define PMIX_TOOL_DO_NOT_CONNECT            "pmix.tool.nocon"       // (bool) the tool wants to use internal PMIx support, but does
                                                                    //        not want to connect to a PMIx server
                                                                    //        from the specified processes to this tool
#define PMIX_TOOL_CONNECT_OPTIONAL          "pmix.tool.conopt"      // (bool) tool shall connect to a server if available, but otherwise
                                                                    //        continue to operate unconnected
#define PMIX_LAUNCHER                       "pmix.tool.launcher"    // (bool) tool is a launcher and needs rendezvous files created
#define PMIX_LAUNCHER_RENDEZVOUS_FILE       "pmix.tool.lncrnd"      // (char*) Pathname of file where connection info is to be stored
#define PMIX_TOOL_ATTACHMENT_FILE           "pmix.tool.attach"      // (char*) File containing connection info to be used for attaching to server
#define PMIX_PRIMARY_SERVER                 "pmix.pri.srvr"         // (bool) The server to which the tool is connecting shall be designated
                                                                    //        the primary server once connection has been accomplished.
#define PMIX_NOHUP                          "pmix.nohup"            // (bool) Any processes started on behalf of the calling tool (or the
                                                                    //        specified namespace, if such specification is included in the
                                                                    //        list of attributes) should continue after the tool disconnects
                                                                    //        from its server
#define PMIX_LAUNCHER_DAEMON                "pmix.lnch.dmn"         // (char*) Path to executable that is to be used as the backend daemon
                                                                    //        for the launcher. This replaces the launcher's own daemon with
                                                                    //        the specified executable. Note that the user is therefore
                                                                    //        responsible for ensuring compatibility of the specified
                                                                    //        executable and the host launcher.
#define PMIX_EXEC_AGENT                     "pmix.exec.agnt"        // (char*) Path to executable that the launcher's backend daemons are to
                                                                    //        fork/exec in place of the actual application processes. The
                                                                    //        launcher's daemon shall pass the full command line of the
                                                                    //        application on the command line of the exec agent, which shall
                                                                    //        not connect back to the launcher's daemon. The exec agent is
                                                                    //        responsible for exec'ing the specified application process in
                                                                    //        its own place.
#define PMIX_LAUNCH_DIRECTIVES              "pmix.lnch.dirs"        // (pmix_data_array_t*) Array of pmix_info_t containing directives for
                                                                    //        the launcher - a convenience attribute for retrieving all
                                                                    //        directives with a single call to PMIx_Get

/* identification attributes */
#define PMIX_USERID                         "pmix.euid"             // (uint32_t) effective user id
#define PMIX_GRPID                          "pmix.egid"             // (uint32_t) effective group id
#define PMIX_VERSION_INFO                   "pmix.version"          // (char*) PMIx version of contactor
#define PMIX_REQUESTOR_IS_TOOL              "pmix.req.tool"         // (bool) requesting process is a tool
#define PMIX_REQUESTOR_IS_CLIENT            "pmix.req.client"       // (bool) requesting process is a client process
#define PMIX_PSET_NAME                      "pmix.pset.nm"          // (char*) The name of the newly defined process set.
#define PMIX_PSET_NAMES                     "pmix.pset.nms"         // (pmix_data_array_t*) Returns an array of string names of the
                                                                    //          process sets in which the given process is a member.
#define PMIX_PSET_MEMBERS                   "pmix.pset.mems"        // (pmix_data_array_t*) An array of pmix_proc_t containing
                                                                    //          the members of the newly defined process set.
#define PMIX_REINCARNATION                  "pmix.reinc"            // (uint32_t) number of times this process has been instantiated - i.e.,
                                                                    //            tracks the number of times it has been restarted

/* model attributes */
#define PMIX_PROGRAMMING_MODEL              "pmix.pgm.model"        // (char*) programming model being initialized (e.g., "MPI" or "OpenMP")
#define PMIX_MODEL_LIBRARY_NAME             "pmix.mdl.name"         // (char*) programming model implementation ID (e.g., "OpenMPI" or "MPICH")
#define PMIX_MODEL_LIBRARY_VERSION          "pmix.mld.vrs"          // (char*) programming model version string (e.g., "2.1.1")
#define PMIX_THREADING_MODEL                "pmix.threads"          // (char*) threading model used (e.g., "pthreads")
#define PMIX_MODEL_NUM_THREADS              "pmix.mdl.nthrds"       // (uint64_t) number of active threads being used by the model
#define PMIX_MODEL_NUM_CPUS                 "pmix.mdl.ncpu"         // (uint64_t) number of cpus being used by the model
#define PMIX_MODEL_CPU_TYPE                 "pmix.mdl.cputype"      // (char*) granularity - "hwthread", "core", etc.
#define PMIX_MODEL_PHASE_NAME               "pmix.mdl.phase"        // (char*) user-assigned name for a phase in the application execution - e.g.,
                                                                    //         "cfd reduction"
#define PMIX_MODEL_PHASE_TYPE               "pmix.mdl.ptype"        // (char*) type of phase being executed - e.g., "matrix multiply"
#define PMIX_MODEL_AFFINITY_POLICY          "pmix.mdl.tap"          // (char*) thread affinity policy - e.g.:
                                                                    //           "master" (thread co-located with master thread),
                                                                    //           "close" (thread located on cpu close to master thread)
                                                                    //           "spread" (threads load-balanced across available cpus)

/* attributes for TCP connections */
#define PMIX_TCP_REPORT_URI                 "pmix.tcp.repuri"       // (char*) output URI - '-' => stdout, '+' => stderr, or filename
#define PMIX_TCP_URI                        "pmix.tcp.uri"          // (char*) URI of server to connect to, or file:<name of file containing it>
#define PMIX_TCP_IF_INCLUDE                 "pmix.tcp.ifinclude"    // (char*) comma-delimited list of devices and/or CIDR notation
#define PMIX_TCP_IF_EXCLUDE                 "pmix.tcp.ifexclude"    // (char*) comma-delimited list of devices and/or CIDR notation
#define PMIX_TCP_IPV4_PORT                  "pmix.tcp.ipv4"         // (int) IPv4 port to be used
#define PMIX_TCP_IPV6_PORT                  "pmix.tcp.ipv6"         // (int) IPv6 port to be used
#define PMIX_TCP_DISABLE_IPV4               "pmix.tcp.disipv4"      // (bool) true to disable IPv4 family
#define PMIX_TCP_DISABLE_IPV6               "pmix.tcp.disipv6"      // (bool) true to disable IPv6 family


/* general proc-level attributes */
#define PMIX_CPUSET                         "pmix.cpuset"           // (char*) String representation of bitmap applied to process upon launch
#define PMIX_CPUSET_BITMAP                  "pmix.bitmap"           // (pmix_cpuset_t*) Bitmap applied to process at launch
#define PMIX_CREDENTIAL                     "pmix.cred"             // (char*) security credential assigned to proc
#define PMIX_SPAWNED                        "pmix.spawned"          // (bool) true if this proc resulted from a call to PMIx_Spawn
#define PMIX_NODE_OVERSUBSCRIBED            "pmix.ndosub"           // (bool) true if number of procs from this job on this node
                                                                    //        exceeds the number of slots allocated to it

/* scratch directory locations for use by applications */
#define PMIX_TMPDIR                         "pmix.tmpdir"           // (char*) top-level tmp dir assigned to session
#define PMIX_NSDIR                          "pmix.nsdir"            // (char*) sub-tmpdir assigned to namespace
#define PMIX_PROCDIR                        "pmix.pdir"             // (char*) sub-nsdir assigned to proc
#define PMIX_TDIR_RMCLEAN                   "pmix.tdir.rmclean"     // (bool)  Resource Manager will clean session directories


/* information about relative ranks as assigned by the RM */
#define PMIX_CLUSTER_ID                     "pmix.clid"             // (char*) a string name for the cluster this proc is executing on
#define PMIX_PROCID                         "pmix.procid"           // (pmix_proc_t*) process identifier
#define PMIX_NSPACE                         "pmix.nspace"           // (char*) nspace of a job
#define PMIX_JOBID                          "pmix.jobid"            // (char*) jobid assigned by scheduler
#define PMIX_APPNUM                         "pmix.appnum"           // (uint32_t) app number within the job
#define PMIX_RANK                           "pmix.rank"             // (pmix_rank_t) process rank within the job
#define PMIX_GLOBAL_RANK                    "pmix.grank"            // (pmix_rank_t) rank spanning across all jobs in this session
#define PMIX_APP_RANK                       "pmix.apprank"          // (pmix_rank_t) rank within this app
#define PMIX_NPROC_OFFSET                   "pmix.offset"           // (pmix_rank_t) starting global rank of this job
#define PMIX_LOCAL_RANK                     "pmix.lrank"            // (uint16_t) rank on this node within this job
#define PMIX_NODE_RANK                      "pmix.nrank"            // (uint16_t) rank on this node spanning all jobs
#define PMIX_PACKAGE_RANK                   "pmix.pkgrank"          // (uint16_t) rank within this job on the package where this proc resides
#define PMIX_LOCALLDR                       "pmix.lldr"             // (pmix_rank_t) lowest rank on this node within this job
#define PMIX_APPLDR                         "pmix.aldr"             // (pmix_rank_t) lowest rank in this app within this job
#define PMIX_PROC_PID                       "pmix.ppid"             // (pid_t) pid of specified proc
#define PMIX_SESSION_ID                     "pmix.session.id"       // (uint32_t) session identifier
#define PMIX_NODE_LIST                      "pmix.nlist"            // (char*) comma-delimited list of nodes running procs for the specified nspace
#define PMIX_ALLOCATED_NODELIST             "pmix.alist"            // (char*) comma-delimited list of all nodes in this allocation regardless of
                                                                    //         whether or not they currently host procs.
#define PMIX_HOSTNAME                       "pmix.hname"            // (char*) name of the host the specified proc is on
#define PMIX_HOSTNAME_ALIASES               "pmix.alias"            // (char*) comma-delimited list of names by which this node is known
#define PMIX_HOSTNAME_KEEP_FQDN             "pmix.fqdn"             // (bool) FQDN hostnames are being retained
#define PMIX_NODEID                         "pmix.nodeid"           // (uint32_t) node identifier where the specified proc is located
#define PMIX_LOCAL_PEERS                    "pmix.lpeers"           // (char*) comma-delimited string of ranks on this node within the specified nspace
#define PMIX_LOCAL_PROCS                    "pmix.lprocs"           // (pmix_data_array_t*) array of pmix_proc_t of procs on the specified node
#define PMIX_LOCAL_CPUSETS                  "pmix.lcpus"            // (char*) colon-delimited cpusets of local peers within the specified nspace
#define PMIX_PARENT_ID                      "pmix.parent"           // (pmix_proc_t*) identifier of the process that called PMIx_Spawn
                                                                    //                to launch this proc's application
#define PMIX_EXIT_CODE                      "pmix.exit.code"        // (int) exit code returned when proc terminated

/* size info */
#define PMIX_UNIV_SIZE                      "pmix.univ.size"        // (uint32_t) #slots in this session
#define PMIX_JOB_SIZE                       "pmix.job.size"         // (uint32_t) #procs in this job
#define PMIX_JOB_NUM_APPS                   "pmix.job.napps"        // (uint32_t) #apps in this job
#define PMIX_APP_SIZE                       "pmix.app.size"         // (uint32_t) #procs in this application
#define PMIX_LOCAL_SIZE                     "pmix.local.size"       // (uint32_t) #procs in this job on this node
#define PMIX_NODE_SIZE                      "pmix.node.size"        // (uint32_t) #procs across all jobs on this node
#define PMIX_MAX_PROCS                      "pmix.max.size"         // (uint32_t) max #procs for this job
#define PMIX_NUM_SLOTS                      "pmix.num.slots"        // (uint32_t) #slots allocated
#define PMIX_NUM_NODES                      "pmix.num.nodes"        // (uint32_t) #nodes currently hosting processes in the specified realm.
#define PMIX_NUM_ALLOCATED_NODES            "pmix.num.anodes"       // (uint32_t) #nodes in the specified realm regardless of whether or
                                                                    //            not they currently host processes.

/* Memory info */
#define PMIX_AVAIL_PHYS_MEMORY              "pmix.pmem"             // (uint64_t) total available physical memory on this node
#define PMIX_DAEMON_MEMORY                  "pmix.dmn.mem"          // (float) Mbytes of memory currently used by daemon
#define PMIX_CLIENT_AVG_MEMORY              "pmix.cl.mem.avg"       // (float) Average Mbytes of memory used by client processes


/* topology info */
#define PMIX_TOPOLOGY2                      "pmix.topo2"            // (pmix_topology_t*) pointer to a PMIx topology object
#define PMIX_LOCALITY_STRING                "pmix.locstr"           // (char*) string describing a proc's location


/* request-related info */
#define PMIX_COLLECT_DATA                   "pmix.collect"          // (bool) collect data and return it at the end of the operation
#define PMIX_ALL_CLONES_PARTICIPATE         "pmix.clone.part"       // (bool) All clones of the calling process must participate in the collective operation.
#define PMIX_COLLECT_GENERATED_JOB_INFO     "pmix.collect.gen"      // (bool) Collect all job-level information (i.e., reserved keys) that was locally
                                                                    //        generated by PMIx servers. Some job-level information (e.g., distance between
                                                                    //        processes and fabric devices) is best determined on a distributed basis as it
                                                                    //        primarily pertains to local processes. Should remote processes need to access
                                                                    //        the information, it can either be obtained collectively using the PMIx_Fence
                                                                    //        operation with this directive, or can be retrieved one peer at a time using
                                                                    //        PMIx_Get without first having performed the job-wide collection.
#define PMIX_TIMEOUT                        "pmix.timeout"          // (int) time in sec before specified operation should time out (0 => infinite)
#define PMIX_IMMEDIATE                      "pmix.immediate"        // (bool) specified operation should immediately return an error from the PMIx
                                                                    //        server if requested data cannot be found - do not request it from
                                                                    //        the host RM
#define PMIX_WAIT                           "pmix.wait"             // (int) caller requests that the server wait until at least the specified
                                                                    //       #values are found (0 => all and is the default)
#define PMIX_NOTIFY_COMPLETION              "pmix.notecomp"         // (bool) notify parent process upon termination of child job
#define PMIX_RANGE                          "pmix.range"            // (pmix_data_range_t) value for calls to publish/lookup/unpublish or for
                                                                    //        monitoring event notifications
#define PMIX_PERSISTENCE                    "pmix.persist"          // (pmix_persistence_t) value for calls to publish
#define PMIX_DATA_SCOPE                     "pmix.scope"            // (pmix_scope_t) scope of the data to be found in a PMIx_Get call
#define PMIX_OPTIONAL                       "pmix.optional"         // (bool) look only in the client's local data store for the requested value - do
                                                                    //        not request data from the server if not found
#define PMIX_GET_STATIC_VALUES              "pmix.get.static"       // (bool) Request that the data be returned in the provided storage location
#define PMIX_GET_POINTER_VALUES             "pmix.get.pntrs"        // (bool) Request that any pointers in the returned value point directly
                                                                    //       to values in the key-value store
#define PMIX_EMBED_BARRIER                  "pmix.embed.barrier"    // (bool) execute a blocking fence operation before executing the
                                                                    //        specified operation
#define PMIX_JOB_TERM_STATUS                "pmix.job.term.status"  // (pmix_status_t) status returned upon job termination
#define PMIX_PROC_TERM_STATUS               "pmix.proc.term.status" // (pmix_status_t) status returned upon process termination
#define PMIX_PROC_STATE_STATUS              "pmix.proc.state"       // (pmix_proc_state_t) process state
#define PMIX_GET_REFRESH_CACHE              "pmix.get.refresh"      // (bool) when retrieving data for a remote process, refresh the existing
                                                                    //        local data cache for the process in case new values have been
                                                                    //        put and committed by it since the last refresh
#define PMIX_ACCESS_PERMISSIONS             "pmix.aperms"           // (pmix_data_array_t*) Define access permissions for the published
                                                                    //        data. The value shall contain an array of pmix_info_t structs
                                                                    //        containing the specified permissions.
#define PMIX_ACCESS_USERIDS                 "pmix.auids"            // (pmix_data_array_t*) Array of effective UIDs that are allowed to
                                                                    //        access the published data
#define PMIX_ACCESS_GRPIDS                  "pmix.agids"            // (pmix_data_array_t*) Array of effective GIDs that are allowed to
                                                                    //        access the published data
#define PMIX_WAIT_FOR_CONNECTION            "pmix.wait.conn"        // (bool) wait until the specified connection has been made
#define PMIX_QUALIFIED_VALUE                "pmix.qual.val"         // (pmix_data_array_t*) Value being provided consists of the primary
                                                                    //        key-value pair in first position, followed by one or more
                                                                    //        key-value qualifiers to be used when subsequently retrieving
                                                                    //        the primary value


/* attributes used by host server to pass data to/from the server convenience library - the
 * data will then be parsed and provided to the local clients. Not generally accessible by users */
#define PMIX_REGISTER_NODATA                "pmix.reg.nodata"       // (bool) Registration is for nspace only, do not copy job data
#define PMIX_NODE_MAP                       "pmix.nmap"             // (char*) regex of nodes containing procs for this job
#define PMIX_NODE_MAP_RAW                   "pmix.nmap.raw"         // (char*) comma-delimited list of nodes containing procs for this job
#define PMIX_PROC_MAP                       "pmix.pmap"             // (char*) regex describing procs on each node within this job
#define PMIX_PROC_MAP_RAW                   "pmix.pmap.raw"         // (char*) semi-colon delimited list of strings, each string containing
                                                                    //         a comma-delimited list of ranks on the corresponding node
#define PMIX_ANL_MAP                        "pmix.anlmap"           // (char*) process mapping in ANL notation (used in PMI-1/PMI-2)
#define PMIX_APP_MAP_TYPE                   "pmix.apmap.type"       // (char*) type of mapping used to layout the application (e.g., cyclic)
#define PMIX_APP_MAP_REGEX                  "pmix.apmap.regex"      // (char*) regex describing the result of the mapping
#define PMIX_REQUIRED_KEY                   "pmix.req.key"          // (char*) key the user needs prior to responding from a dmodex request
#define PMIX_LOCAL_COLLECTIVE_STATUS        "pmix.loc.col.st"       // (pmix_status_t) status code for local collective operation being
                                                                    //         reported to host by server library
#define PMIX_SORTED_PROC_ARRAY              "pmix.sorted.parr"      // (bool) Proc array being passed has been sorted


/* event handler registration and notification info keys */
#define PMIX_EVENT_HDLR_NAME                "pmix.evname"           // (char*) string name identifying this handler
#define PMIX_EVENT_HDLR_FIRST               "pmix.evfirst"          // (bool) invoke this event handler before any other handlers
#define PMIX_EVENT_HDLR_LAST                "pmix.evlast"           // (bool) invoke this event handler after all other handlers have been called
#define PMIX_EVENT_HDLR_FIRST_IN_CATEGORY   "pmix.evfirstcat"       // (bool) invoke this event handler before any other handlers in this category
#define PMIX_EVENT_HDLR_LAST_IN_CATEGORY    "pmix.evlastcat"        // (bool) invoke this event handler after all other handlers in this category have been called
#define PMIX_EVENT_HDLR_BEFORE              "pmix.evbefore"         // (char*) put this event handler immediately before the one specified in the (char*) value
#define PMIX_EVENT_HDLR_AFTER               "pmix.evafter"          // (char*) put this event handler immediately after the one specified in the (char*) value
#define PMIX_EVENT_HDLR_PREPEND             "pmix.evprepend"        // (bool) prepend this handler to the precedence list within its category
#define PMIX_EVENT_HDLR_APPEND              "pmix.evappend"         // (bool) append this handler to the precedence list within its category
#define PMIX_EVENT_CUSTOM_RANGE             "pmix.evrange"          // (pmix_data_array_t*) array of pmix_proc_t defining range of event notification
#define PMIX_EVENT_AFFECTED_PROC            "pmix.evproc"           // (pmix_proc_t*) single proc that was affected
#define PMIX_EVENT_AFFECTED_PROCS           "pmix.evaffected"       // (pmix_data_array_t*) array of pmix_proc_t defining affected procs
#define PMIX_EVENT_NON_DEFAULT              "pmix.evnondef"         // (bool) event is not to be delivered to default event handlers
#define PMIX_EVENT_RETURN_OBJECT            "pmix.evobject"         // (void*) object to be returned whenever the registered cbfunc is invoked
                                                                    //     NOTE: the object will _only_ be returned to the process that
                                                                    //           registered it
#define PMIX_EVENT_DO_NOT_CACHE             "pmix.evnocache"        // (bool) instruct the PMIx server not to cache the event
#define PMIX_EVENT_SILENT_TERMINATION       "pmix.evsilentterm"     // (bool) do not generate an event when this job normally terminates
#define PMIX_EVENT_PROXY                    "pmix.evproxy"          // (pmix_proc_t*) PMIx server that sourced the event
#define PMIX_EVENT_TEXT_MESSAGE             "pmix.evtext"           // (char*) text message suitable for output by recipient - e.g., describing
                                                                    //         the cause of the event
#define PMIX_EVENT_TIMESTAMP                "pmix.evtstamp"         // (time_t) System time when the associated event occurred.
#define PMIX_EVENT_ONESHOT                  "pmix.evone"            // (bool) when registering, indicate that this event handler is to be deleted
                                                                    //        after being invoked

/* fault tolerance-related events */
#define PMIX_EVENT_TERMINATE_SESSION        "pmix.evterm.sess"      // (bool) RM intends to terminate session
#define PMIX_EVENT_TERMINATE_JOB            "pmix.evterm.job"       // (bool) RM intends to terminate this job
#define PMIX_EVENT_TERMINATE_NODE           "pmix.evterm.node"      // (bool) RM intends to terminate all procs on this node
#define PMIX_EVENT_TERMINATE_PROC           "pmix.evterm.proc"      // (bool) RM intends to terminate just this process
#define PMIX_EVENT_ACTION_TIMEOUT           "pmix.evtimeout"        // (int) time in sec before RM will execute error response

/* attributes used to describe "spawn" directives */
#define PMIX_PERSONALITY                    "pmix.pers"             // (char*) name of personality to use
#define PMIX_HOST                           "pmix.host"             // (char*) comma-delimited list of hosts to use for spawned procs
#define PMIX_HOSTFILE                       "pmix.hostfile"         // (char*) hostfile to use for spawned procs
#define PMIX_ADD_HOST                       "pmix.addhost"          // (char*) comma-delimited list of hosts to add to allocation
#define PMIX_ADD_HOSTFILE                   "pmix.addhostfile"      // (char*) hostfile to add to existing allocation
#define PMIX_PREFIX                         "pmix.prefix"           // (char*) prefix to use for starting spawned procs
#define PMIX_WDIR                           "pmix.wdir"             // (char*) working directory for spawned procs
#define PMIX_WDIR_USER_SPECIFIED            "pmix.wdir.user"        // (bool) User specified the working directory
#define PMIX_DISPLAY_MAP                    "pmix.dispmap"          // (bool) display placement map upon spawn
#define PMIX_DISPLAY_MAP_DETAILED           "pmix.dispmapdet"       // (bool) display a highly detailed placement map upon spawn
#define PMIX_DISPLAY_ALLOCATION             "pmix.dispalloc"        // (bool) display the resource allocation
#define PMIX_DISPLAY_TOPOLOGY               "pmix.disptopo"         // (char*) comma-delimited list of hosts whose topology is
                                                                    //         to be displayed
#define PMIX_PPR                            "pmix.ppr"              // (char*) #procs to spawn on each identified resource
#define PMIX_MAPBY                          "pmix.mapby"            // (char*) mapping policy
#define PMIX_RANKBY                         "pmix.rankby"           // (char*) ranking policy
#define PMIX_BINDTO                         "pmix.bindto"           // (char*) binding policy
#define PMIX_PRELOAD_BIN                    "pmix.preloadbin"       // (bool) preload binaries
#define PMIX_PRELOAD_FILES                  "pmix.preloadfiles"     // (char*) comma-delimited list of files to pre-position
#define PMIX_STDIN_TGT                      "pmix.stdin"            // (pmix_proc_t*) proc that is to receive stdin
                                                                    //                (PMIX_RANK_WILDCARD = all in given nspace)
#define PMIX_DEBUGGER_DAEMONS               "pmix.debugger"         // (bool) spawned app consists of debugger daemons
#define PMIX_COSPAWN_APP                    "pmix.cospawn"          // (bool) designated app is to be spawned as a disconnected
                                                                    //        job - i.e., not part of the "comm_world" of the job
#define PMIX_COLOCATE_PROCS                 "pmix.colproc"          // (pmix_data_array_t*) Array of pmix_proc_t identifying the procs
                                                                    //        with which the new job's procs are to be colocated
#define PMIX_COLOCATE_NPERPROC              "pmix.colnum.proc"      // (uint16_t) Number of procs to colocate with each identified proc
#define PMIX_COLOCATE_NPERNODE              "pmix.colnum.node"      // (uint16_t) Number of procs to colocate on the node of each identified proc
#define PMIX_SET_SESSION_CWD                "pmix.ssncwd"           // (bool) set the application's current working directory to
                                                                    //        the session working directory assigned by the RM
#define PMIX_INDEX_ARGV                     "pmix.indxargv"         // (bool) mark the argv with the rank of the proc
#define PMIX_CPUS_PER_PROC                  "pmix.cpuperproc"       // (uint32_t) #cpus to assign to each rank
#define PMIX_NO_PROCS_ON_HEAD               "pmix.nolocal"          // (bool) do not place procs on the head node
#define PMIX_NO_OVERSUBSCRIBE               "pmix.noover"           // (bool) do not oversubscribe the cpus
#define PMIX_REPORT_BINDINGS                "pmix.repbind"          // (bool) report bindings of the individual procs
#define PMIX_CPU_LIST                       "pmix.cpulist"          // (char*) list of cpus to use for this job
#define PMIX_JOB_RECOVERABLE                "pmix.recover"          // (bool) application supports recoverable operations
#define PMIX_JOB_CONTINUOUS                 "pmix.continuous"       // (bool) application is continuous, all failed procs should
                                                                        //        be immediately restarted
#define PMIX_MAX_RESTARTS                   "pmix.maxrestarts"      // (uint32_t) max number of times to restart a job
#define PMIX_FWD_STDIN                      "pmix.fwd.stdin"        // (bool) forward the stdin from this process to the target processes
#define PMIX_FWD_STDOUT                     "pmix.fwd.stdout"       // (bool) forward stdout from the spawned processes to this process (typically used by a tool)
#define PMIX_FWD_STDERR                     "pmix.fwd.stderr"       // (bool) forward stderr from the spawned processes to this process (typically used by a tool)
#define PMIX_FWD_STDDIAG                    "pmix.fwd.stddiag"      // (bool) if a diagnostic channel exists, forward any output on it
                                                                    //        from the spawned processes to this process (typically used by a tool)
#define PMIX_SPAWN_TOOL                     "pmix.spwn.tool"        // (bool) job being spawned is a tool
#define PMIX_CMD_LINE                       "pmix.cmd.line"         // (char*) command line executing in the specified nspace
#define PMIX_FORKEXEC_AGENT                 "pmix.fe.agnt"          // (char*) command line of fork/exec agent to be used for starting
                                                                    //         local processes
#define PMIX_JOB_TIMEOUT                    "pmix.job.time"         // (int) time in sec before job should time out (0 => infinite)
#define PMIX_SPAWN_TIMEOUT                  "pmix.sp.time"          // (int) time in sec before spawn operation should time out (0 => infinite)
                                                                    //       Logically equivalent to passing the PMIX_TIMEOUT attribute to the
                                                                    //       PMIx_Spawn API, it is provided as a separate attribute to distinguish
                                                                    //       it from the PMIX_JOB_TIMEOUT attribute
#define PMIX_TIMEOUT_STACKTRACES            "pmix.tim.stack"        // (bool) include process stacktraces in timeout report from a job
#define PMIX_TIMEOUT_REPORT_STATE           "pmix.tim.state"        // (bool) report process states in timeout report from a job
#define PMIX_APP_ARGV                       "pmix.app.argv"         // (char*) consolidated argv passed to the spawn command for the given app
#define PMIX_NOTIFY_JOB_EVENTS              "pmix.note.jev"         // (bool) Requests that the launcher generate the PMIX_EVENT_JOB_START,
                                                                    //        PMIX_LAUNCH_COMPLETE, and PMIX_EVENT_JOB_END events. Each event is to
                                                                    //        include at least the namespace of the corresponding job and a
                                                                    //        PMIX_EVENT_TIMESTAMP indicating the time the event occurred.
#define PMIX_NOTIFY_PROC_TERMINATION        "pmix.noteproc"         // (bool) Requests that the launcher generate the PMIX_EVENT_PROC_TERMINATED
                                                                    //        event whenever a process either normally or abnormally terminates.
#define PMIX_NOTIFY_PROC_ABNORMAL_TERMINATION   "pmix.noteabproc"   // (bool) Requests that the launcher generate the PMIX_EVENT_PROC_TERMINATED
                                                                    //        event only when a process abnormally terminates.
#define PMIX_ENVARS_HARVESTED               "pmix.evar.hvstd"       // (bool) Envars have been harvested by the spawn requestor
#define PMIX_RUNTIME_OPTIONS                "pmix.runopt"           // (char*) Environment-specific runtime directives that control job behavior
#define PMIX_ABORT_NON_ZERO_TERM            "pmix.abnz"             // (bool) Abort the spawned job if any process terminates with non-zero status
#define PMIX_DO_NOT_LAUNCH                  "pmix.dnl"              // (bool) Execute all procedures to prepare the requested job for launch,
                                                                    //        but do not launch it. Typically combined with the PMIX_DISPLAY_MAP
                                                                    //        or PMIX_DISPLAY_MAP_DETAILED for debugging purposes.
#define PMIX_SHOW_LAUNCH_PROGRESS           "pmix.showprog"         // (bool) Provide periodic progress reports on job launch procedure (e.g., after
                                                                    //        every 100 processes have been spawned)
#define PMIX_AGGREGATE_HELP                 "pmix.agg.help"         // (bool) Aggregate help messages, reporting each unique help message once
                                                                    //        accompanied by the number of processes that reported it
#define PMIX_REPORT_CHILD_SEP               "pmix.rptchildsep"      // (bool) Report the exit status of any child jobs spawned by the primary job
                                                                    //        separately. If false, then the final exit status reported will be
                                                                    //        zero if the primary job and all spawned jobs exit normally, or the
                                                                    //        first non-zero status returned by either primary or child jobs.


/* query keys - value type shown is the type of the value that will be RETURNED by that key  */
#define PMIX_QUERY_SUPPORTED_KEYS           "pmix.qry.keys"         // (char*) returns comma-delimited list of keys supported by the query
                                                                    //         function. NO QUALIFIERS
#define PMIX_QUERY_NAMESPACES               "pmix.qry.ns"           // (char*) returns a comma-delimited list of active namespaces. NO QUALIFIERS
#define PMIX_QUERY_NAMESPACE_INFO           "pmix.qry.nsinfo"       // (pmix_data_array_t*) returns an array of active nspace information - each
                                                                    //        element will contain an array including the namespace plus the
                                                                    //        command line of the application executing within it
                                                                    //        SUPPORTED QUALIFIERS: PMIX_NSPACE of specific nspace whose info
                                                                    //        is being requested
#define PMIX_QUERY_JOB_STATUS               "pmix.qry.jst"          // (pmix_status_t) returns status of a specified currently executing job
                                                                    //         REQUIRES a PMIX_NSPACE qualifier indicating the nspace being queried
#define PMIX_QUERY_QUEUE_LIST               "pmix.qry.qlst"         // (char*) request a comma-delimited list of scheduler queues. NO QUALIFIERS
#define PMIX_QUERY_QUEUE_STATUS             "pmix.qry.qst"          // (pmix_data_array_t*) returns array where each element contains the name and
                                                                    //         status of a scheduler queue
                                                                    //        SUPPORTED QUALIFIERS: PMIX_ALLOC_QUEUE naming specific queue whose status
                                                                    //        is being requested
#define PMIX_QUERY_PROC_TABLE               "pmix.qry.ptable"       // (pmix_data_array_t*) returns (pmix_data_array_t*) an array of pmix_proc_info_t
                                                                    //         REQUIRES a PMIX_NSPACE qualifier indicating the nspace being queried
#define PMIX_QUERY_LOCAL_PROC_TABLE         "pmix.qry.lptable"      // (pmix_data_array_t*) returns (pmix_data_array_t*) an array of pmix_proc_info_t
                                                                    //         of pmix_proc_info_t for procs in job on same node
                                                                    //         REQUIRES a PMIX_NSPACE qualifier indicating the nspace being queried
#define PMIX_QUERY_AUTHORIZATIONS           "pmix.qry.auths"        // (pmix_data_array_t*) return operations tool is authorized to perform. The contents
                                                                    //         of the array elements have not yet been standardized. NO QUALIFIERS
#define PMIX_QUERY_SPAWN_SUPPORT            "pmix.qry.spawn"        // (char*) return a comma-delimited list of supported spawn attributes. NO QUALIFIERS
#define PMIX_QUERY_DEBUG_SUPPORT            "pmix.qry.debug"        // (char*) return a comma-delimited list of supported debug attributes. NO QUALIFIERS
#define PMIX_QUERY_MEMORY_USAGE             "pmix.qry.mem"          // (pmix_data_array_t*) return info on memory usage for the procs indicated in the qualifiers
                                                                    //        SUPPORTED QUALIFIERS: PMIX_NSPACE/PMIX_RANK, or PMIX_PROCID of specific proc(s)
                                                                    //        whose info is being requested
#define PMIX_QUERY_ALLOC_STATUS             "pmix.query.alloc"      // (char*) return a string reporting status of an allocation request
                                                                    //         REQUIRES a PMIX_ALLOC_ID qualifier indicating the allocation request being queried
#define PMIX_TIME_REMAINING                 "pmix.time.remaining"   // (uint32_t) returns number of seconds remaining in allocation
                                                                    //         for the specified nspace (defaults to allocation containing the caller)
                                                                    //         SUPPORTED QUALIFIERS: PMIX_NSPACE of the nspace whose info is being requested
#define PMIX_QUERY_NUM_PSETS                "pmix.qry.psetnum"      // (size_t) returns the number of psets defined
                                                                    //          in the specified range (defaults to session)
                                                                    //         SUPPORTED QUALIFIERS: PMIX_RANGE whose info is being requested
#define PMIX_QUERY_PSET_NAMES               "pmix.qry.psets"        // (char*) returns a comma-delimited list of the names of the
                                                                    //         psets defined in the specified range (defaults to session)
                                                                    //         SUPPORTED QUALIFIERS: PMIX_RANGE whose info is being requested
#define PMIX_QUERY_PSET_MEMBERSHIP          "pmix.qry.pmems"        // (pmix_data_array_t*) Return an array of pmix_proc_t containing the members of
                                                                    //         the specified process set.
#define PMIX_QUERY_NUM_GROUPS               "pmix.qry.pgrpnum"      // (size_t) Return the number of process groups defined in the specified range
                                                                    //         (defaults to session). OPTIONAL QUALIFERS: PMIX_RANGE.
#define PMIX_QUERY_GROUP_NAMES              "pmix.qry.pgrp"         // (pmix_data_array_t*) Return a pmix_data_array_t containing an array of string
                                                                    //         names of the process groups defined in the specified range (defaults
                                                                    //         to session). OPTIONAL QUALIFERS: PMIX_RANGE
#define PMIX_QUERY_GROUP_MEMBERSHIP         "pmix.qry.pgrpmems"     // (pmix_data_array_t*) Return a pmix_data_array_t of pmix_proc_t containing
                                                                    //         the members of the specified process group. REQUIRED QUALIFIERS:
                                                                    //         PMIX_GROUP_ID.
#define PMIX_QUERY_ATTRIBUTE_SUPPORT        "pmix.qry.attrs"        // (pmix_data_array_t*) returns array of pmix_info_t where each element consists
                                                                    //         of a key containing the name of the function, and an array of pmix_regattr_t
                                                                    //         detailing the attribute support for that function
                                                                    //         SUPPORTED QUALIFIERS: PMIX_CLIENT_FUNCTIONS, PMIX_SERVER_FUNCTIONS,
                                                                    //         PMIX_TOOL_FUNCTIONS, and/or PMIX_HOST_FUNCTIONS
#define PMIX_CLIENT_FUNCTIONS               "pmix.client.fns"       // (char*) returns a comma-delimited list of supported PMIx client functions. NO QUALIFIERS
#define PMIX_SERVER_FUNCTIONS               "pmix.srvr.fns"         // (char*) returns a comma-delimited list of supported PMIx server functions. NO QUALIFIERS
#define PMIX_TOOL_FUNCTIONS                 "pmix.tool.fns"         // (char*) returns a comma-delimited list of supported PMIx tool functions. NO QUALIFIERS
#define PMIX_HOST_FUNCTIONS                 "pmix.host.fns"         // (char*) returns a comma-delimited list of PMIx functions supported by the host environment
#define PMIX_QUERY_AVAIL_SERVERS            "pmix.qry.asrvrs"       // (pmix_data_array_t*) array of pmix_info_t, each element containing an array of
                                                                    //         pmix_info_t of available data for servers on this node
                                                                    //         to which the caller might be able to connect. NO QUALIFIERS
#define PMIX_QUERY_QUALIFIERS               "pmix.qry.quals"        // (pmix_data_array_t*) Contains an array of qualifiers that were included in the
                                                                    //         query that produced the provided results. This attribute is solely for
                                                                    //         reporting purposes and cannot be used in PMIx_Get or other query
                                                                    //         operations
#define PMIX_QUERY_RESULTS                  "pmix.qry.res"          // (pmix_data_array_t*) Contains an array of query results for a given pmix_query_t passed to the
                                                                    //         PMIx_Query_info APIs. If qualifiers were included in the query, then the first element
                                                                    //         of the array shall be the PMIX_QUERY_QUALIFIERS attribute containing those qualifiers.
                                                                    //         Each of the remaining elements of the array is a pmix_info_t containing the query key
                                                                    //         and the corresponding value returned by the query. This attribute is solely for
                                                                    //         reporting purposes and cannot be used in PMIx_Get or other query operations


/* query qualifiers - these are used to provide information to narrow/modify the query. Value type shown is the type of data expected
 * to be provided with the key */
#define PMIX_QUERY_REFRESH_CACHE            "pmix.qry.rfsh"         // (bool) retrieve updated information from server
                                                                    //        to update local cache
#define PMIX_QUERY_LOCAL_ONLY               "pmix.qry.local"        // (bool) constrain the query to local information only
#define PMIX_QUERY_REPORT_AVG               "pmix.qry.avg"          // (bool) report average values
#define PMIX_QUERY_REPORT_MINMAX            "pmix.qry.minmax"       // (bool) report minimum and maximum value
#define PMIX_CLIENT_ATTRIBUTES              "pmix.client.attrs"     // (char*) comma-delimited list of functions, including "all"
                                                                    //        when used in a query, indicates whether or not to include
                                                                    //        attributes supported by the PMIx client library
#define PMIX_SERVER_ATTRIBUTES              "pmix.srvr.attrs"       // (char*) comma-delimited list of functions, including "all"
                                                                    //        when used in a query, indicates whether or not to include
                                                                    //        attributes supported by the PMIx server library
#define PMIX_HOST_ATTRIBUTES                "pmix.host.attrs"       // (char*) comma-delimited list of functions, including "all"
                                                                    //        when used in a query, indicates whether or not to include
                                                                    //        attributes supported by the host environment
#define PMIX_TOOL_ATTRIBUTES                "pmix.tool.attrs"       // (char*) comma-delimited list of functions, including "all"
                                                                    //        when used in a query, indicates whether or not to include
                                                                    //        attributes supported by the PMIx tool library
#define PMIX_QUERY_SUPPORTED_QUALIFIERS     "pmix.qry.quals"        // (bool) return comma-delimited list of qualifiers supported by
                                                                    //        a query on the provided key, instead of actually performing
                                                                    //        the query on the key.


/* PMIx_Get information retrieval attributes */
#define PMIX_SESSION_INFO                   "pmix.ssn.info"         // (bool) Return information about the specified session. If information
                                                                    //        about a session other than the one containing the requesting
                                                                    //        process is desired, then the attribute array must contain a
                                                                    //        PMIX_SESSION_ID attribute identifying the desired target.
#define PMIX_JOB_INFO                       "pmix.job.info"         // (bool) Return information about the specified job or namespace. If
                                                                    //        information about a job or namespace other than the one containing
                                                                    //        the requesting process is desired, then the attribute array must
                                                                    //        contain a PMIX_JOBID or PMIX_NSPACE attribute identifying the
                                                                    //        desired target. Similarly, if information is requested about a
                                                                    //        job or namespace in a session other than the one containing the
                                                                    //        requesting process, then an attribute identifying the target
                                                                    //        session must be provided.
#define PMIX_APP_INFO                       "pmix.app.info"         // (bool) Return information about the specified application. If information
                                                                    //        about an application other than the one containing the requesting
                                                                    //        process is desired, then the attribute array must contain a
                                                                    //        PMIX_APPNUM attribute identifying the desired target. Similarly,
                                                                    //        if information is requested about an application in a job or session
                                                                    //        other than the one containing the requesting process, then attributes
                                                                    //        identifying the target job and/or session must be provided.
#define PMIX_NODE_INFO                      "pmix.node.info"        // (bool) Return information about the specified node. If information about a
                                                                    //        node other than the one containing the requesting process is desired,
                                                                    //        then the attribute array must contain either the PMIX_NODEID or
                                                                    //        PMIX_HOSTNAME attribute identifying the desired target.


/* information storage attributes */
#define PMIX_SESSION_INFO_ARRAY             "pmix.ssn.arr"          // (pmix_data_array_t*) Provide an array of pmix_info_t containing
                                                                    //        session-level information. The PMIX_SESSION_ID attribute is required
                                                                    //        to be included in the array.
#define PMIX_JOB_INFO_ARRAY                 "pmix.job.arr"          // (pmix_data_array_t*) Provide an array of pmix_info_t containing job-level
                                                                    //        information. Information is registered one job (aka namespace) at a time
                                                                    //        via the PMIx_server_register_nspace API. Thus, there is no requirement that
                                                                    //        the array contain either the PMIX_NSPACE or PMIX_JOBID attributes, though
                                                                    //        either or both of them may be included.
#define PMIX_APP_INFO_ARRAY                 "pmix.app.arr"          // (pmix_data_array_t*) Provide an array of pmix_info_t containing app-level
                                                                    //        information. The PMIX_NSPACE or PMIX_JOBID attributes of the job containing
                                                                    //        the application, plus its PMIX_APPNUM attribute, are required to be
                                                                    //        included in the array.
#define PMIX_PROC_INFO_ARRAY                "pmix.pdata"            // (pmix_data_array_t*) Provide an array of pmix_info_t containing process-realm
                                                                    //        information. The PMIX_RANK and PMIX_NSPACE attributes, or the
                                                                    //        PMIX_PROCID attribute, are required to be included in the array when
                                                                    //        the array is not included as part of a call to
                                                                    //        PMIx_server_register_nspace - i.e., when the job containing the process
                                                                    //        is ambiguous. All three may be included if desired. When the array is
                                                                    //        included in some broader structure that identifies the job, then only
                                                                    //        the PMIX_RANK or the PMIX_PROCID attribute must be included (the others
                                                                    //        are optional).
#define PMIX_NODE_INFO_ARRAY                "pmix.node.arr"         // (pmix_data_array_t*) Provide an array of pmix_info_t containing node-level
                                                                    //        information. At a minimum, either the PMIX_NODEID or PMIX_HOSTNAME
                                                                    //        attribute is required to be included in the array, though both may be
                                                                    //        included.
#define PMIX_SERVER_INFO_ARRAY              "pmix.srv.arr"          // (pmix_data_array_t*) array of data on a given server, starting with its nspace


/* log attributes */
#define PMIX_LOG_SOURCE                     "pmix.log.source"       // (pmix_proc_t*) ID of source of the log request
#define PMIX_LOG_STDERR                     "pmix.log.stderr"       // (char*) log string to stderr
#define PMIX_LOG_STDOUT                     "pmix.log.stdout"       // (char*) log string to stdout
#define PMIX_LOG_SYSLOG                     "pmix.log.syslog"       // (char*) log message to syslog - defaults to ERROR priority. Will log
                                                                    //         to global syslog if available, otherwise to local syslog
#define PMIX_LOG_LOCAL_SYSLOG               "pmix.log.lsys"         // (char*) log msg to local syslog - defaults to ERROR priority
#define PMIX_LOG_GLOBAL_SYSLOG              "pmix.log.gsys"         // (char*) forward data to system "master" and log msg to that syslog
#define PMIX_LOG_SYSLOG_PRI                 "pmix.log.syspri"       // (int) syslog priority level

#define PMIX_LOG_TIMESTAMP                  "pmix.log.tstmp"        // (time_t) timestamp for log report
#define PMIX_LOG_GENERATE_TIMESTAMP         "pmix.log.gtstmp"       // (bool) generate timestamp for log
#define PMIX_LOG_TAG_OUTPUT                 "pmix.log.tag"          // (bool) label the output stream with the channel name (e.g., "stdout")
#define PMIX_LOG_TIMESTAMP_OUTPUT           "pmix.log.tsout"        // (bool) print timestamp in output string
#define PMIX_LOG_XML_OUTPUT                 "pmix.log.xml"          // (bool) print the output stream in xml format
#define PMIX_LOG_ONCE                       "pmix.log.once"         // (bool) only log this once with whichever channel can first support it
#define PMIX_LOG_MSG                        "pmix.log.msg"          // (pmix_byte_object_t) message blob to be sent somewhere
#define PMIX_LOG_KEY                        "pmix.log.key"          // (char*) key to a logging message
#define PMIX_LOG_VAL                        "pmix.log.val"          // (char*) value to a logging message
#define PMIX_LOG_AGG                        "pmix.log.agg"          // (bool) Whether to aggregate and prevent duplicate logging messages
                                                                    //        based on key value pairs.

#define PMIX_LOG_EMAIL                      "pmix.log.email"        // (pmix_data_array_t*) log via email based on array of pmix_info_t
                                                                    //         containing directives
#define PMIX_LOG_EMAIL_ADDR                 "pmix.log.emaddr"       // (char*) comma-delimited list of email addresses that are to recv msg
#define PMIX_LOG_EMAIL_SENDER_ADDR          "pmix.log.emfaddr"      // (char*) return email address of sender
#define PMIX_LOG_EMAIL_SUBJECT              "pmix.log.emsub"        // (char*) subject line for email
#define PMIX_LOG_EMAIL_MSG                  "pmix.log.emmsg"        // (char*) msg to be included in email
#define PMIX_LOG_EMAIL_SERVER               "pmix.log.esrvr"        // (char*) hostname (or IP addr) of estmp server
#define PMIX_LOG_EMAIL_SRVR_PORT            "pmix.log.esrvrprt"     // (int32_t) port the email server is listening to

#define PMIX_LOG_GLOBAL_DATASTORE           "pmix.log.gstore"       // (bool) log the provided data to a global datastore
#define PMIX_LOG_JOB_RECORD                 "pmix.log.jrec"         // (bool) log the provided information to the RM's job record
#define PMIX_LOG_PROC_TERMINATION           "pmix.logproc"          // (bool) Requests that the launcher log the PMIX_EVENT_PROC_TERMINATED event
                                                                    //        whenever a process either normally or abnormally terminates.
#define PMIX_LOG_PROC_ABNORMAL_TERMINATION  "pmix.logabproc"        // (bool) Requests that the launcher log the PMIX_EVENT_PROC_TERMINATED event
                                                                    //        only when a process abnormally terminates.
#define PMIX_LOG_JOB_EVENTS                 "pmix.log.jev"          // (bool) Requests that the launcher log the PMIX_EVENT_JOB_START,
                                                                    //        PMIX_LAUNCH_COMPLETE, and PMIX_EVENT_JOB_END events using PMIx_Log
#define PMIX_LOG_COMPLETION                 "pmix.logcomp"          // (bool) Requests that the launcher log the PMIX_EVENT_JOB_END event
                                                                    //        for normal or abnormal termination of the spawned job using
                                                                    //        PMIx_Log. The event shall include the returned status code
                                                                    //        (PMIX_JOB_TERM_STATUS) for the corresponding job; the identity
                                                                    //        (PMIX_PROCID) and exit status (PMIX_EXIT_CODE) of the first failed
                                                                    //        process, if applicable; and a PMIX_EVENT_TIMESTAMP indicating the time
                                                                    //        the termination occurred.


/* debugger attributes */
#define PMIX_DEBUG_STOP_ON_EXEC             "pmix.dbg.exec"         // (varies) stop specified rank(s) on exec and notify ready-to-debug
                                                                    //        Can be any of three data types:
                                                                    //           (a) bool - true indicating all ranks, false indicating none
                                                                    //           (b) pmix_rank_t - the rank of one proc, or WILDCARD for all
                                                                    //           (c) a pmix_data_array_t if an array of individual processes
                                                                    //               are specified
#define PMIX_DEBUG_STOP_IN_INIT             "pmix.dbg.init"         // (varies) stop specified rank(s) in PMIx_Init and notify ready-to-debug
                                                                    //        Can be any of three data types:
                                                                    //           (a) bool - true indicating all ranks, false indicating none
                                                                    //           (b) pmix_rank_t - the rank of one proc, or WILDCARD for all
                                                                    //           (c) a pmix_data_array_t if an array of individual processes
                                                                    //               are specified
#define PMIX_DEBUG_STOP_IN_APP              "pmix.dbg.notify"       // (varies) direct specified ranks to stop at application-specific point and
                                                                    //        notify ready-to-debug. Can be any of three data types:
                                                                    //           (a) bool - true indicating all ranks, false indicating none
                                                                    //           (b) pmix_rank_t - the rank of one proc, or WILDCARD for all
                                                                    //           (c) a pmix_data_array_t if an array of individual processes
                                                                    //               are specified
#define PMIX_BREAKPOINT                     "pmix.brkpnt"           // (char*) string ID of the breakpoint where the process(es) is(are) waiting
#define PMIX_DEBUG_TARGET                   "pmix.dbg.tgt"          // (pmix_proc_t*) Identifier of proc(s) to be debugged
#define PMIX_DEBUG_DAEMONS_PER_PROC         "pmix.dbg.dpproc"       // (uint16_t) Number of debugger daemons to be spawned per application
                                                                    //        process. The launcher is to pass the identifier of the namespace to
                                                                    //        be debugged by including the PMIX_DEBUG_TARGET attribute in the
                                                                    //        daemon's job-level information. The debugger daemons spawned on a
                                                                    //        given node are responsible for self-determining their specific
                                                                    //        target process(es) - e.g., by referencing their own PMIX_LOCAL_RANK
                                                                    //        in the daemon debugger job versus the corresponding PMIX_LOCAL_RANK
                                                                    //        of the target processes on the node.
#define PMIX_DEBUG_DAEMONS_PER_NODE         "pmix.dbg.dpnd"         // (uint16_t) Number of debugger daemons to be spawned on each node where the
                                                                    //        target job is executing. The launcher is to pass the identifier of
                                                                    //        the namespace to be debugged by including the PMIX_DEBUG_TARGET
                                                                    //        attribute in the daemon's job-level information. The debugger
                                                                    //        daemons spawned on a given node are responsible for
                                                                    //        self-determining their specific target process(es) - e.g., by
                                                                    //        referencing their own PMIX_LOCAL_RANK in the daemon debugger job
                                                                    //        versus the corresponding PMIX_LOCAL_RANK of the target processes on
                                                                    //        the node.


/* Resource Manager identification */
#define PMIX_RM_NAME                        "pmix.rm.name"          // (char*) string name of the resource manager
#define PMIX_RM_VERSION                     "pmix.rm.version"       // (char*) RM version string

/* environmental variable operation attributes */
#define PMIX_SET_ENVAR                      "pmix.envar.set"          // (pmix_envar_t*) set the envar to the given value,
                                                                      //                 overwriting any pre-existing one
#define PMIX_ADD_ENVAR                      "pmix.envar.add"          // (pmix_envar_t*) add envar, but do not overwrite any existing one
#define PMIX_UNSET_ENVAR                    "pmix.envar.unset"        // (char*) unset the envar, if present
#define PMIX_PREPEND_ENVAR                  "pmix.envar.prepnd"       // (pmix_envar_t*) prepend the given value to the
                                                                      //                 specified envar using the separator
                                                                      //                 character, creating the envar if it doesn't already exist
#define PMIX_APPEND_ENVAR                   "pmix.envar.appnd"        // (pmix_envar_t*) append the given value to the specified
                                                                      //                 envar using the separator character,
                                                                      //                 creating the envar if it doesn't already exist
#define PMIX_FIRST_ENVAR                    "pmix.envar.first"        // (pmix_envar_t*) ensure the given value appears first in the
                                                                      //                 specified envar using the separator
                                                                      //                 character, creating the envar if it doesn't already exist

/* attributes relating to allocations */
#define PMIX_ALLOC_REQ_ID                   "pmix.alloc.reqid"      // (char*) User-provided string identifier for this allocation request
                                                                    //         which can later be used to query status of the request.
#define PMIX_ALLOC_ID                       "pmix.alloc.id"         // (char*) A string identifier (provided by the host environment) for
                                                                    //         the resulting allocation which can later be used to reference
                                                                    //         the allocated resources in, for example, a call to PMIx_Spawn
#define PMIX_ALLOC_NUM_NODES                "pmix.alloc.nnodes"     // (uint64_t) number of nodes
#define PMIX_ALLOC_NODE_LIST                "pmix.alloc.nlist"      // (char*) regex of specific nodes
#define PMIX_ALLOC_NUM_CPUS                 "pmix.alloc.ncpus"      // (uint64_t) number of cpus
#define PMIX_ALLOC_NUM_CPU_LIST             "pmix.alloc.ncpulist"   // (char*) regex of #cpus for each node
#define PMIX_ALLOC_CPU_LIST                 "pmix.alloc.cpulist"    // (char*) regex of specific cpus indicating the cpus involved.
#define PMIX_ALLOC_MEM_SIZE                 "pmix.alloc.msize"      // (float) number of Mbytes
#define PMIX_ALLOC_FABRIC                   "pmix.alloc.net"        // (pmix_data_array_t*) Array of pmix_info_t describing
                                                                    //         fabric resource request. This must include at least:
                                                                    //           * PMIX_ALLOC_FABRIC_ID
                                                                    //           * PMIX_ALLOC_FABRIC_TYPE
                                                                    //           * PMIX_ALLOC_FABRIC_ENDPTS
                                                                    //         plus whatever other descriptors are desired
#define PMIX_ALLOC_FABRIC_ID                "pmix.alloc.netid"      // (char*) key to be used when accessing this requested fabric allocation. The
                                                                    //         allocation will be returned/stored as a pmix_data_array_t of
                                                                    //         pmix_info_t indexed by this key and containing at least one
                                                                    //         entry with the same key and the allocated resource description.
                                                                    //         The type of the included value depends upon the fabric
                                                                    //         support. For example, a TCP allocation might consist of a
                                                                    //         comma-delimited string of socket ranges such as
                                                                    //         "32000-32100,33005,38123-38146". Additional entries will consist
                                                                    //         of any provided resource request directives, along with their
                                                                    //         assigned values. Examples include:
                                                                    //           * PMIX_ALLOC_FABRIC_TYPE - the type of resources provided
                                                                    //           * PMIX_ALLOC_FABRIC_PLANE - if applicable, what plane the
                                                                    //               resources were assigned from
                                                                    //           * PMIX_ALLOC_FABRIC_QOS - the assigned QoS
                                                                    //           * PMIX_ALLOC_BANDWIDTH - the allocated bandwidth
                                                                    //           * PMIX_ALLOC_FABRIC_SEC_KEY - a security key for the requested
                                                                    //               fabric allocation
                                                                    //         NOTE: the assigned values may differ from those requested,
                                                                    //         especially if the "required" flag was not set in the request
#define PMIX_ALLOC_BANDWIDTH                "pmix.alloc.bw"         // (float) Mbits/sec
#define PMIX_ALLOC_FABRIC_QOS               "pmix.alloc.netqos"     // (char*) quality of service level
#define PMIX_ALLOC_TIME                     "pmix.alloc.time"       // (uint32_t) time in seconds that the allocation shall remain valid
#define PMIX_ALLOC_FABRIC_TYPE              "pmix.alloc.nettype"    // (char*) type of desired transport (e.g., tcp, udp)
#define PMIX_ALLOC_FABRIC_PLANE             "pmix.alloc.netplane"   // (char*) id string for the NIC (aka plane) to be used for this allocation
                                                                    //         (e.g., CIDR for Ethernet)
#define PMIX_ALLOC_FABRIC_ENDPTS            "pmix.alloc.endpts"     // (size_t) number of endpoints to allocate per process
#define PMIX_ALLOC_FABRIC_ENDPTS_NODE       "pmix.alloc.endpts.nd"  // (size_t) number of endpoints to allocate per node
#define PMIX_ALLOC_FABRIC_SEC_KEY           "pmix.alloc.nsec"       // (pmix_byte_object_t) fabric security key
#define PMIX_ALLOC_QUEUE                    "pmix.alloc.queue"      // (char*) name of queue being referenced


/* job control attributes */
#define PMIX_JOB_CTRL_ID                    "pmix.jctrl.id"         // (char*) provide a string identifier for this request
#define PMIX_JOB_CTRL_PAUSE                 "pmix.jctrl.pause"      // (bool) pause the specified processes
#define PMIX_JOB_CTRL_RESUME                "pmix.jctrl.resume"     // (bool) "un-pause" the specified processes
#define PMIX_JOB_CTRL_CANCEL                "pmix.jctrl.cancel"     // (char*) cancel the specified request
                                                                    //         (NULL => cancel all requests from this requestor)
#define PMIX_JOB_CTRL_KILL                  "pmix.jctrl.kill"       // (bool) forcibly terminate the specified processes and cleanup
#define PMIX_JOB_CTRL_RESTART               "pmix.jctrl.restart"    // (char*) restart the specified processes using the given checkpoint ID
#define PMIX_JOB_CTRL_CHECKPOINT            "pmix.jctrl.ckpt"       // (char*) checkpoint the specified processes and assign the given ID to it
#define PMIX_JOB_CTRL_CHECKPOINT_EVENT      "pmix.jctrl.ckptev"     // (bool) use event notification to trigger process checkpoint
#define PMIX_JOB_CTRL_CHECKPOINT_SIGNAL     "pmix.jctrl.ckptsig"    // (int) use the given signal to trigger process checkpoint
#define PMIX_JOB_CTRL_CHECKPOINT_TIMEOUT    "pmix.jctrl.ckptsig"    // (int) time in seconds to wait for checkpoint to complete
#define PMIX_JOB_CTRL_CHECKPOINT_METHOD     "pmix.jctrl.ckmethod"   // (pmix_data_array_t*) array of pmix_info_t declaring each
                                                                    //      method and value supported by this application
#define PMIX_JOB_CTRL_SIGNAL                "pmix.jctrl.sig"        // (int) send given signal to specified processes
#define PMIX_JOB_CTRL_PROVISION             "pmix.jctrl.pvn"        // (char*) regex identifying nodes that are to be provisioned
#define PMIX_JOB_CTRL_PROVISION_IMAGE       "pmix.jctrl.pvnimg"     // (char*) name of the image that is to be provisioned
#define PMIX_JOB_CTRL_PREEMPTIBLE           "pmix.jctrl.preempt"    // (bool) job can be pre-empted
#define PMIX_JOB_CTRL_TERMINATE             "pmix.jctrl.term"       // (bool) politely terminate the specified procs
#define PMIX_REGISTER_CLEANUP               "pmix.reg.cleanup"      // (char*) comma-delimited list of files to
                                                                    //         be removed upon process termination
#define PMIX_REGISTER_CLEANUP_DIR           "pmix.reg.cleanupdir"   // (char*) comma-delimited list of directories to
                                                                    //         be removed upon process termination
#define PMIX_CLEANUP_RECURSIVE              "pmix.clnup.recurse"    // (bool) recursively cleanup all subdirectories under the
                                                                    //        specified one(s)
#define PMIX_CLEANUP_EMPTY                  "pmix.clnup.empty"      // (bool) only remove empty subdirectories
#define PMIX_CLEANUP_IGNORE                 "pmix.clnup.ignore"     // (char*) comma-delimited list of filenames that are not
                                                                    //         to be removed
#define PMIX_CLEANUP_LEAVE_TOPDIR           "pmix.clnup.lvtop"      // (bool) when recursively cleaning subdirs, do not remove
                                                                    //        the top-level directory (the one given in the
                                                                    //        cleanup request)

/* monitoring attributes */
#define PMIX_MONITOR_ID                     "pmix.monitor.id"       // (char*) provide a string identifier for this request
#define PMIX_MONITOR_CANCEL                 "pmix.monitor.cancel"   // (char*) identifier to be canceled (NULL = cancel all
                                                                    //         monitoring for this process)
#define PMIX_MONITOR_APP_CONTROL            "pmix.monitor.appctrl"  // (bool) the application desires to control the response to
                                                                    //        a monitoring event
#define PMIX_MONITOR_HEARTBEAT              "pmix.monitor.mbeat"    // (bool) register to have the server monitor the requestor for heartbeats
#define PMIX_SEND_HEARTBEAT                 "pmix.monitor.beat"     // (bool) send heartbeat to local server
#define PMIX_MONITOR_HEARTBEAT_TIME         "pmix.monitor.btime"    // (uint32_t) time in seconds before declaring heartbeat missed
#define PMIX_MONITOR_HEARTBEAT_DROPS        "pmix.monitor.bdrop"    // (uint32_t) number of heartbeats that can be missed before
                                                                    //            generating the event
#define PMIX_MONITOR_FILE                   "pmix.monitor.fmon"     // (char*) register to monitor file for signs of life
#define PMIX_MONITOR_FILE_SIZE              "pmix.monitor.fsize"    // (bool) monitor size of given file is growing to determine app is running
#define PMIX_MONITOR_FILE_ACCESS            "pmix.monitor.faccess"  // (char*) monitor time since last access of given file to determine app is running
#define PMIX_MONITOR_FILE_MODIFY            "pmix.monitor.fmod"     // (char*) monitor time since last modified of given file to determine app is running
#define PMIX_MONITOR_FILE_CHECK_TIME        "pmix.monitor.ftime"    // (uint32_t) time in seconds between checking file
#define PMIX_MONITOR_FILE_DROPS             "pmix.monitor.fdrop"    // (uint32_t) number of file checks that can be missed before
                                                                    //            generating the event

/* security attributes */
#define PMIX_CRED_TYPE                      "pmix.sec.ctype"        // (char*) when passed in PMIx_Get_credential, a prioritized,
                                                                    // comma-delimited list of desired credential types for use
                                                                    // in environments where multiple authentication mechanisms
                                                                    // may be available. When returned in a callback function, a
                                                                    // string identifier of the credential type
#define PMIX_CRYPTO_KEY                     "pmix.sec.key"          // (pmix_byte_object_t) blob containing crypto key


/* IO Forwarding Attributes */
#define PMIX_IOF_CACHE_SIZE                 "pmix.iof.csize"        // (uint32_t) requested size of the server cache in bytes for each specified channel.
                                                                    //            By default, the server is allowed (but not required) to drop
                                                                    //            all bytes received beyond the max size
#define PMIX_IOF_DROP_OLDEST                "pmix.iof.old"          // (bool) in an overflow situation, drop the oldest bytes to make room in the cache
#define PMIX_IOF_DROP_NEWEST                "pmix.iof.new"          // (bool) in an overflow situation, drop any new bytes received until room becomes
                                                                    //        available in the cache (default)
#define PMIX_IOF_BUFFERING_SIZE             "pmix.iof.bsize"        // (uint32_t) basically controls grouping of IO on the specified channel(s) to
                                                                    //            avoid being called every time a bit of IO arrives. The library
                                                                    //            will execute the callback whenever the specified number of bytes
                                                                    //            becomes available. Any remaining buffered data will be "flushed"
                                                                    //            upon call to deregister the respective channel
#define PMIX_IOF_BUFFERING_TIME             "pmix.iof.btime"        // (uint32_t) max time in seconds to buffer IO before delivering it. Used in conjunction
                                                                    //            with buffering size, this prevents IO from being held indefinitely
                                                                    //            while waiting for another payload to arrive
#define PMIX_IOF_COMPLETE                   "pmix.iof.cmp"          // (bool) indicates whether or not the specified IO channel has been closed
                                                                    //        by the source
#define PMIX_IOF_PUSH_STDIN                 "pmix.iof.stdin"        // (bool) Used by a tool to request that the PMIx library collect
                                                                    //        the tool's stdin and forward it to the procs specified in
                                                                    //        the PMIx_IOF_push call
#define PMIX_IOF_TAG_OUTPUT                 "pmix.iof.tag"          // (bool) Tag output with the [local jobid,rank] and channel it comes from
#define PMIX_IOF_TAG_DETAILED_OUTPUT        "pmix.iof.tagdet"       // (bool) Tag output with the [local jobid,rank][hostname:pid] and channel it comes from
#define PMIX_IOF_TAG_FULLNAME_OUTPUT        "pmix.iof.tagfull"      // (bool) Tag output with the [nspace,rank] and channel it comes from
#define PMIX_IOF_RANK_OUTPUT                "pmix.iof.rank"         // (bool) Tag output with the rank it came from
#define PMIX_IOF_TIMESTAMP_OUTPUT           "pmix.iof.ts"           // (bool) Timestamp output
#define PMIX_IOF_MERGE_STDERR_STDOUT        "pmix.iof.mrg"          // (bool) merge stdout and stderr streams from application procs
#define PMIX_IOF_XML_OUTPUT                 "pmix.iof.xml"          // (bool) Format output in XML
#define PMIX_IOF_OUTPUT_TO_FILE             "pmix.iof.file"         // (char*) direct application output into files of form
                                                                    //         "<filename>.rank" with both stdout and stderr redirected into it
#define PMIX_IOF_FILE_PATTERN               "pmix.iof.fpt"          // (bool) Specified output file is to be treated as a pattern and not
                                                                    //        automatically annotated by nspace, rank, or other parameters
#define PMIX_IOF_OUTPUT_TO_DIRECTORY        "pmix.iof.dir"          // (char*) direct application output into files of form
                                                                    //         "<directory>/<jobid>/rank.<rank>/stdout[err]"
#define PMIX_IOF_FILE_ONLY                  "pmix.iof.fonly"        // (bool) output only into designated files - do not also output
                                                                    //        a copy to stdout/stderr
#define PMIX_IOF_COPY                       "pmix.iof.cpy"          // (bool) Requests that the host environment deliver a copy of the
                                                                    //        specified output stream(s) to the tool, letting the stream(s)
                                                                    //        continue to also be delivered to the default location. This
                                                                    //        allows the tool to tap into the output stream(s) without
                                                                    //        redirecting it from its current final destination.
#define PMIX_IOF_REDIRECT                   "pmix.iof.redir"        // (bool) Requests that the host environment intercept the specified
                                                                    //        output stream(s) and deliver it to the requesting tool instead
                                                                    //        of its current final destination. This might be used, for
                                                                    //        example, during a debugging procedure to avoid injection of
                                                                    //        debugger-related output into the application's results file.
                                                                    //        The original output stream(s) destination is restored upon
                                                                    //        termination of the tool.
#define PMIX_IOF_LOCAL_OUTPUT               "pmix.iof.local"        // (bool) Write output streams to local stdout/err
#define PMIX_IOF_OUTPUT_RAW                 "pmix.iof.raw"          // (bool) Do not buffer output to be written as complete lines - output
                                                                    //        characters as the stream delivers them

/* Attributes for controlling contents of application setup data */
#define PMIX_SETUP_APP_ENVARS               "pmix.setup.env"        // (bool) harvest and include relevant envars
#define PMIX_SETUP_APP_NONENVARS            "pmix.setup.nenv"       // (bool) include all non-envar data
#define PMIX_SETUP_APP_ALL                  "pmix.setup.all"        // (bool) include all relevant data

/* Attributes supporting the PMIx Groups APIs */
#define PMIX_GROUP_ID                       "pmix.grp.id"           // (char*) user-provided group identifier
#define PMIX_GROUP_LEADER                   "pmix.grp.ldr"          // (bool) this process is the leader of the group
#define PMIX_GROUP_OPTIONAL                 "pmix.grp.opt"          // (bool) participation is optional - do not return an error if any of the
                                                                    //        specified processes terminate without having joined. The default
                                                                    //        is false
#define PMIX_GROUP_NOTIFY_TERMINATION       "pmix.grp.notterm"      // (bool) notify remaining members when another member terminates without first
                                                                    //        leaving the group. The default is false
#define PMIX_GROUP_FT_COLLECTIVE            "pmix.grp.ftcoll"       // (bool) adjust internal tracking for terminated processes. Default is false
#define PMIX_GROUP_MEMBERSHIP               "pmix.grp.mbrs"         // (pmix_data_array_t*) array of group member ID's
#define PMIX_GROUP_ASSIGN_CONTEXT_ID        "pmix.grp.actxid"       // (bool) request that the RM assign a unique numerical (size_t) ID to this group
#define PMIX_GROUP_CONTEXT_ID               "pmix.grp.ctxid"        // (size_t) context ID assigned to group
#define PMIX_GROUP_LOCAL_ONLY               "pmix.grp.lcl"          // (bool) group operation only involves local procs
#define PMIX_GROUP_ENDPT_DATA               "pmix.grp.endpt"        // (pmix_byte_object_t) data collected to be shared during construction
#define PMIX_GROUP_NAMES                    "pmix.pgrp.nm"          // (pmix_data_array_t*) Returns an array of string names of the process groups
                                                                    //        in which the given process is a member.
#define PMIX_GROUP_INFO                     "pmix.grp.info"         // (pmix_data_array_t*) Array of pmix_info_t containing data that is to be
                                                                    //        shared across all members of a group during group construction
#define PMIX_GROUP_LOCAL_CID                "pmix.grp.lclid"        // (size_t) local context ID for the specified process member of a group
#define PMIX_GROUP_ADD_MEMBERS              "pmix.grp.add"          // (pmix_data_array_t*) Array of pmix_proc_t identifying procs that are not
                                                                    //        included in the membership specified in the procs array passed to
                                                                    //        the PMIx_Group_construct[_nb] call, but are to be included in the
                                                                    //        final group. The identified procs will be sent an invitation to
                                                                    //        join the group during the construction procedure. This is used when
                                                                    //        some members of the proposed group do not know the full membership
                                                                    //        and therefore cannot include all members in the call to construct.

/* Storage-Related Attributes */
#define PMIX_QUERY_STORAGE_LIST             "pmix.strg.list"       // (char*) return comma-delimited list of identifiers for all available storage systems
#define PMIX_STORAGE_CAPACITY_LIMIT         "pmix.strg.cap"        // (uint64_t) return overall capacity (in Megabytes[base2]) of specified storage system
#define PMIX_STORAGE_OBJECT_LIMIT           "pmix.strg.obj"        // (uint64_t) return overall limit on number of objects (e.g., inodes) of specified storage system

#define PMIX_STORAGE_ID                     "pmix.strg.id"         // (char*) identifier of the storage system being referenced
#define PMIX_STORAGE_PATH                   "pmix.strg.path"       // (char*) Mount point corresponding to a specified storage ID
#define PMIX_STORAGE_TYPE                   "pmix.strg.type"       // (char*) Qualifier indicating the type of storage being referenced by a query
                                                                   //         (e.g., lustre, gpfs, online, fabric-attached, ...)
#define PMIX_STORAGE_ACCESSIBILITY          "pmix.strg.access"     // (pmix_storage_accessibility_t) Accessibility level of the storage system
                                                                   //         (e.g., within same node, within same session)
#define PMIX_STORAGE_ACCESS_TYPE            "pmix.strg.atype"      // (pmix_storage_access_type_t) Qualifier describing the type of storage access to return
                                                                   //       information for (e.g., for qualifying PMIX_STORAGE_BW_CUR, PMIX_STORAGE_IOPS_CUR,
                                                                   //       or PMIX_STORAGE_SUGGESTED_XFER_SIZE attributes)
#define PMIX_STORAGE_BW_CUR                 "pmix.strg.bwcur"      // (double) Observed bandwidth (in bytes/sec) for storage system - provided as a
                                                                   //       recently observed bandwidth value, with the exact measurement interval
                                                                   //       depending on the storage system and/or PMIx library implementation
#define PMIX_STORAGE_BW_MAX                 "pmix.strg.bwmax"      // (double) Maximum bandwidth (in bytes/sec) for storage system - provided as the
                                                                   //       theoretical maximum or the maximum observed bandwidth value
#define PMIX_STORAGE_CAPACITY_USED          "pmix.strg.capuse"     // (double) Overall used capacity (in bytes) for the storage system
#define PMIX_STORAGE_IOPS_CUR               "pmix.strg.iopscur"    // (double) Observed IOPS (in I/O operations per second) for storage system - provided
                                                                   //       as a recently observed IOPS value, with the exact measurement interval depending
                                                                   //       on the storage system and/or PMIx library implementation
#define PMIX_STORAGE_IOPS_MAX               "pmix.strg.iopsmax"    // (double) Maximum IOPS (in I/O operations per second) for storage system - provided
                                                                   //       as the theoretical maximum or the maximum observed IOPS value
#define PMIX_STORAGE_MEDIUM                 "pmix.strg.medium"     // (pmix_storage_medium_t) Types of storage mediums utilized by the storage system
                                                                   //       (e.g., SSDs, HDDs, tape)
#define PMIX_STORAGE_MINIMAL_XFER_SIZE      "pmix.strg.minxfer"    // (double) Minimal transfer size (in bytes) for the storage system - this is the
                                                                   //       storage system's atomic unit of transfer (e.g., block size)
#define PMIX_STORAGE_OBJECTS_USED           "pmix.strg.objuse"     // (uint64_t) Overall used number of objects (e.g., inodes) for the storage system
#define PMIX_STORAGE_PERSISTENCE            "pmix.strg.persist"    // (pmix_storage_persistence_t) Persistence level of the storage system
                                                                   //       (e.g., sratch storage or archive storage)
#define PMIX_STORAGE_SUGGESTED_XFER_SIZE    "pmix.strg.sxfer"      // (double) Suggested transfer size (in bytes) for the storage system
#define PMIX_STORAGE_VERSION                "pmix.strg.ver"        // (char*) Version string for the storage system


/* Fabric-related Attributes */
#define PMIX_FABRIC_COST_MATRIX             "pmix.fab.cm"          // (pointer) Pointer to a two-dimensional array of point-to-point relative
                                                                   //           communication costs expressed as uint16_t values
#define PMIX_FABRIC_GROUPS                  "pmix.fab.grps"        // (char*) A string delineating the group membership of nodes in the system,
                                                                   //         where each fabric group consists of the group number followed by
                                                                   //         a colon and a comma-delimited list of nodes in that group, with the
                                                                   //         groups delimited by semi-colons (e.g.,
                                                                   //         0:node000,node002,node004,node006;1:node001,node003,node005,node007)
#define PMIX_FABRIC_VENDOR                  "pmix.fab.vndr"        // (char*) Name of fabric vendor (e.g., Amazon, Mellanox, HPE, Intel)
#define PMIX_FABRIC_IDENTIFIER              "pmix.fab.id"          // (char*) An identifier for the fabric (e.g., MgmtEthernet, Slingshot-11,
                                                                   //         OmniPath-1)
#define PMIX_FABRIC_INDEX                   "pmix.fab.idx"         // (size_t) The index of the fabric as returned in pmix_fabric_t
#define PMIX_FABRIC_COORDINATES             "pmix.fab.coord"       // (pmix_data_array_t*) Array of pmix_geometry_t fabric coordinates for
                                                                   //          devices on the specified node. The array will contain the
                                                                   //          coordinates of all devices on the node, including values for
                                                                   //          all supported coordinate views. The information for devices
                                                                   //          on the local node shall be provided if the node is not
                                                                   //          specified in the request.
#define PMIX_FABRIC_DEVICE_VENDORID         "pmix.fabdev.vendid"   // (char*) This is a vendor-provided identifier for the device or product.
#define PMIX_FABRIC_NUM_DEVICES             "pmix.fab.nverts"      // (size_t) Total number of fabric devices in the system - corresponds to
                                                                   //          the number of rows or columns in the cost matrix
#define PMIX_FABRIC_DIMS                    "pmix.fab.dims"        // (uint32_t) Number of dimensions in the specified fabric plane/view. If no
                                                                   //          plane is specified in a request, then the dimensions of all planes
                                                                   //          in the overall system will be returned as a pmix_data_array_t
                                                                   //          containing an array of uint32_t values. Default is to
                                                                   //          provide dimensions in logical view.

#define PMIX_FABRIC_PLANE                   "pmix.fab.plane"       // (char*) ID string of a fabric plane (e.g., CIDR for Ethernet). When used as
                                                                   //          a modifier in a request for information, specifies the plane whose
                                                                   //          information is to be returned. When used directly as a key in a
                                                                   //          request, returns a pmix_data_array_t of string
                                                                   //          identifiers for all fabric planes in the overall system.

#define PMIX_FABRIC_SWITCH                  "pmix.fab.switch"      // (char*) ID string of a fabric switch. When used as a modifier in a request
                                                                   //         for information, specifies the switch whose information is to be
                                                                   //         returned. When used directly as a key in a request, returns a
                                                                   //         pmix_data_array_t of string identifiers for all fabric switches in
                                                                   //         the overall system.

#define PMIX_FABRIC_ENDPT                   "pmix.fab.endpt"       // (pmix_data_array_t*) Fabric endpoints for a specified process. As multiple
                                                                   //         endpoints may be assigned to a given process (e.g., in the case
                                                                   //         where multiple devices are associated with a package to which the
                                                                   //         process is bound), the returned values will be provided in a
                                                                   //         pmix_data_array_t of pmix_endpoint_t elements.

#define PMIX_FABRIC_SHAPE                   "pmix.fab.shape"       // (pmix_data_array_t*) The size of each dimension in the specified fabric
                                                                   //         plane/view, returned in a pmix_data_array_t containing an array of
                                                                   //         uint32_t values. The size is defined as the number of elements
                                                                   //         present in that dimension - e.g., the number of devices in one
                                                                   //         dimension of a physical view of a fabric plane. If no plane is
                                                                   //         specified, then the shape of each plane in the overall system will
                                                                   //         be returned in a pmix_data_array_t array where each element is
                                                                   //         itself a two-element array containing the PMIX_FABRIC_PLANE
                                                                   //         followed by that plane's fabric shape. Default is to provide the
                                                                   //         shape in logical view.

#define PMIX_FABRIC_SHAPE_STRING            "pmix.fab.shapestr"    // (char*) Network shape expressed as a string (e.g., "10x12x2"). If no plane
                                                                   //         is specified, then the shape of each plane in the overall system
                                                                   //         will be returned in a pmix_data_array_t array where
                                                                   //         each element is itself a two-element array containing the
                                                                   //         PMIX_FABRIC_PLANE followed by that plane's fabric shape string.
                                                                   //         Default is to provide the shape in logical view.

#define PMIX_SWITCH_PEERS                   "pmix.speers"          // (char*) Peer ranks that share the same switch as the process specified in
                                                                   //         the call to PMIx_Get. Returns a pmix_data_array_t array of
                                                                   //         pmix_info_t results, each element containing the PMIX_SWITCH_PEERS
                                                                   //         key with a three-element pmix_data_array_t array of pmix_info_t
                                                                   //         containing the PMIX_FABRIC_DEVICE_ID of the local fabric device,
                                                                   //         the PMIX_FABRIC_SWITCH identifying the switch to which it is
                                                                   //         connected, and a comma-delimited string of peer ranks sharing the
                                                                   //         switch to which that device is connected.
#define PMIX_FABRIC_DEVICE                  "pmix.fabdev"          // (pmix_data_array_t*) An array of pmix_info_t describing a particular
                                                                   //         fabric device. The first element in the array shall be the
                                                                   //         PMIX_FABRIC_DEVICE_ID of the device
#define PMIX_FABRIC_DEVICES                 "pmix.fab.devs"        // (pmix_data_array_t*) Array of pmix_info_t containing information for all
                                                                   //         devices on the specified node. Each element of the array will contain
                                                                   //         a PMIX_FABRIC_DEVICE entry, which in turn will contain an array of
                                                                   //         information on a given device.
#define PMIX_FABRIC_DEVICE_NAME             "pmix.fabdev.nm"       // (char*) The operating system name associated with the device. This may be
                                                                   //         a logical fabric interface name (e.g. eth0 or eno1) or an absolute
                                                                   //         filename.
#define PMIX_FABRIC_DEVICE_INDEX            "pmix.fabdev.idx"      // (uint32_t) Index of the device within an associated communication cost
                                                                   //         matrix.

#define PMIX_FABRIC_DEVICE_VENDOR           "pmix.fabdev.vndr"     // (char*) Indicates the name of the vendor that distributes the NIC.
#define PMIX_FABRIC_DEVICE_BUS_TYPE         "pmix.fabdev.btyp"     // (char*) The type of bus to which the device is attached (e.g., "PCI",
                                                                   //         "GEN-Z").
#define PMIX_FABRIC_DEVICE_DRIVER           "pmix.fabdev.driver"   // (char*) The name of the driver associated with the device
#define PMIX_FABRIC_DEVICE_FIRMWARE         "pmix.fabdev.fmwr"     // (char*) The device's firmware version
#define PMIX_FABRIC_DEVICE_ADDRESS          "pmix.fabdev.addr"     // (char*) The primary link-level address associated with the device, such as a
                                                                   //         MAC address. If multiple addresses are available, only one will be
                                                                   //         reported.
#define PMIX_FABRIC_DEVICE_COORDINATES      "pmix.fab.coord"       // (pmix_geometry_t) The pmix_geometry_t fabric coordinates for the device, including
                                                                   //          values for all supported coordinate views.
#define PMIX_FABRIC_DEVICE_MTU              "pmix.fabdev.mtu"      // (size_t) The maximum transfer unit of link level frames or packets,
                                                                   //          in bytes.
#define PMIX_FABRIC_DEVICE_SPEED            "pmix.fabdev.speed"    // (size_t) The active link data rate, given in bits per second.
#define PMIX_FABRIC_DEVICE_STATE            "pmix.fabdev.state"    // (pmix_link_state_t) The last available physical port state. Possible values
                                                                   //          are PMIX_LINK_STATE_UNKNOWN, PMIX_LINK_DOWN, and PMIX_LINK_UP, to
                                                                   //          indicate if the port state is unknown or not applicable (unknown),
                                                                   //          inactive (down), or active (up).
#define PMIX_FABRIC_DEVICE_TYPE             "pmix.fabdev.type"     // (char*) Specifies the type of fabric interface currently active on the
                                                                   //         device, such as Ethernet or InfiniBand.
#define PMIX_FABRIC_DEVICE_PCI_DEVID        "pmix.fabdev.pcidevid" // (char*) A node-level unique identifier for a PCI device. Provided only if the
                                                                   //         device is located on a \ac{PCI} bus. The identifier is constructed as
                                                                   //         a four-part tuple delimited by colons comprised of the \ac{PCI} 16-bit
                                                                   //         domain, 8-bit bus, 8-bit device, and 8-bit function IDs, each expressed
                                                                   //         in zero-extended hexadecimal form. Thus, an example identifier might be
                                                                   //         "abc1:0f:23:01". The combination of node identifier PMIX_HOSTNAME or
                                                                   //         PMIX_NODEID and PMIX_FABRIC_DEVICE_PCI_DEVID shall be unique within the
                                                                   //         system.


/* Distance Attributes */
#define PMIX_DEVICE_DISTANCES               "pmix.dev.dist"        // (pmix_data_array_t*) Return an array of pmix_device_distance_t containing the
                                                                   //         minimum and maximum distances of the given process location to all
                                                                   //         devices of the specified type on the local node.
#define PMIX_DEVICE_TYPE                    "pmix.dev.type"        // (pmix_device_type_t) Bitmask specifying the type(s) of device(s) whose
                                                                   //         information is being requested. Only used as a directive/qualifier.
#define PMIX_DEVICE_ID                      "pmix.dev.id"          // (char*) System-wide UUID or node-local OS name of a particular device.


/* Descriptive Attributes */
#define PMIX_MAX_VALUE                      "pmix.descr.maxval"    // (varies) Used in pmix_regattr_t to describe the maximum valid value
                                                                   //          for the associated attribute.
#define PMIX_MIN_VALUE                      "pmix.descr.minval"    // (varies) Used in pmix_regattr_t to describe the minimum valid value
                                                                   //          for the associated attribute.
#define PMIX_ENUM_VALUE                     "pmix.descr.enum"      // (char*) Used in pmix_regattr_t to describe accepted values for the
                                                                   //         associated attribute. Numerical values shall be presented in
                                                                   //         a form convertible to the attribute's declared data type.
                                                                   //         Named values (i.e., values defined by constant names via a
                                                                   //         typical C-language enum declaration) must be provided as
                                                                   //         their numerical equivalent.
#define PMIX_QUERY_STABLE_ABI_VERSION       "pmix.qry.stabiver"    // (char*) The PMIx Standard Stable ABI version supported returned in the form of a comma separated list of "MAJOR.MINOR"
                                                                   //         This attribute can be used with PMIx_Query_info outside of the init/finalize region.
#define PMIX_QUERY_PROVISIONAL_ABI_VERSION  "pmix.qry.prabiver"    // (char*) The PMIx Standard Provisional ABI version supported returned in the form of a comma separated "MAJOR.MINOR"
                                                                   //         This attribute can be used with PMIx_Query_info outside of the init/finalize region.

/****    PROCESS STATE DEFINITIONS    ****/
typedef uint8_t pmix_proc_state_t;
#define PMIX_PROC_STATE_UNDEF                    0  /* undefined process state */
#define PMIX_PROC_STATE_PREPPED                  1  /* process is ready to be launched */
#define PMIX_PROC_STATE_LAUNCH_UNDERWAY          2  /* launch process underway */
#define PMIX_PROC_STATE_RESTART                  3  /* the proc is ready for restart */
#define PMIX_PROC_STATE_TERMINATE                4  /* process is marked for termination */
#define PMIX_PROC_STATE_RUNNING                  5  /* daemon has locally fork'd process */
#define PMIX_PROC_STATE_CONNECTED                6  /* proc connected to PMIx server */
/*
* Define a "boundary" so users can easily and quickly determine
* if a proc is still running or not - any value less than
* this one means that the proc has not terminated
*/
#define PMIX_PROC_STATE_UNTERMINATED            15

#define PMIX_PROC_STATE_TERMINATED              20  /* process has terminated and is no longer running */
/* Define a boundary so users can easily and quickly determine
* if a proc abnormally terminated - leave a little room
* for future expansion
*/
#define PMIX_PROC_STATE_ERROR                   50
/* Define specific error code values */
#define PMIX_PROC_STATE_KILLED_BY_CMD           (PMIX_PROC_STATE_ERROR +  1)  /* process was killed by cmd */
#define PMIX_PROC_STATE_ABORTED                 (PMIX_PROC_STATE_ERROR +  2)  /* process aborted */
#define PMIX_PROC_STATE_FAILED_TO_START         (PMIX_PROC_STATE_ERROR +  3)  /* process failed to start */
#define PMIX_PROC_STATE_ABORTED_BY_SIG          (PMIX_PROC_STATE_ERROR +  4)  /* process aborted by signal */
#define PMIX_PROC_STATE_TERM_WO_SYNC            (PMIX_PROC_STATE_ERROR +  5)  /* process exit'd w/o calling PMIx_Finalize */
#define PMIX_PROC_STATE_COMM_FAILED             (PMIX_PROC_STATE_ERROR +  6)  /* process communication has failed */
#define PMIX_PROC_STATE_SENSOR_BOUND_EXCEEDED   (PMIX_PROC_STATE_ERROR +  7)  /* process exceeded a sensor limit */
#define PMIX_PROC_STATE_CALLED_ABORT            (PMIX_PROC_STATE_ERROR +  8)  /* process called "PMIx_Abort" */
#define PMIX_PROC_STATE_HEARTBEAT_FAILED        (PMIX_PROC_STATE_ERROR +  9)  /* process failed to send heartbeat w/in time limit */
#define PMIX_PROC_STATE_MIGRATING               (PMIX_PROC_STATE_ERROR + 10)  /* process failed and is waiting for resources before restarting */
#define PMIX_PROC_STATE_CANNOT_RESTART          (PMIX_PROC_STATE_ERROR + 11)  /* process failed and cannot be restarted */
#define PMIX_PROC_STATE_TERM_NON_ZERO           (PMIX_PROC_STATE_ERROR + 12)  /* process exited with a non-zero status, indicating abnormal */
#define PMIX_PROC_STATE_FAILED_TO_LAUNCH        (PMIX_PROC_STATE_ERROR + 13)  /* unable to launch process */


/****    JOB STATE DEFINITIONS    ****/
typedef uint8_t pmix_job_state_t;
#define PMIX_JOB_STATE_UNDEF                     0  // undefined process state
#define PMIX_JOB_STATE_AWAITING_ALLOC            1  // Job is waiting for resources to be allocated to it
#define PMIX_JOB_STATE_LAUNCH_UNDERWAY           2  // job launch underway
#define PMIX_JOB_STATE_RUNNING                   3  // all procs have been spawned
#define PMIX_JOB_STATE_SUSPENDED                 4  // job has been suspended
#define PMIX_JOB_STATE_CONNECTED                 5  // all procs have connected to their PMIx server

/*
* Define a "boundary" so users can easily and quickly determine
* if a job is still running or not - any value less than
* this one means that the job has not terminated
*/
#define PMIX_JOB_STATE_UNTERMINATED             15

#define PMIX_JOB_STATE_TERMINATED               20  // job has terminated and is no longer running - typically will
                                                    // be accompanied by the job exit status in response to a query

/* Define a boundary so users can easily and quickly determine
* if a job abnormally terminated - leave a little room
* for future expansion
*/
#define PMIX_JOB_STATE_TERMINATED_WITH_ERROR    50  // job has terminated and is no longer running - typically will
                                                    // be accompanied by a job-related error code in response to a query


/****    PMIX ERROR CONSTANTS    ****/
/* PMIx errors are always negative, with 0 reserved for success */
typedef int pmix_status_t;

/* v1.x error values - must be fixed in place for backward
 * compatibility. Note that some number of these have been
 * deprecated and may not be returned by v2.x and above
 * clients or servers. However, they must always be
 * at least defined to ensure older codes will compile */
#define PMIX_SUCCESS                                 0
#define PMIX_ERROR                                  -1          // general error
/* fault tolerance */
#define PMIX_ERR_PROC_RESTART                       -4
#define PMIX_ERR_PROC_CHECKPOINT                    -5
#define PMIX_ERR_PROC_MIGRATE                       -6
#define PMIX_ERR_EXISTS                             -11
/* communication failures */
#define PMIX_ERR_INVALID_CRED                       -12
#define PMIX_ERR_WOULD_BLOCK                        -15
#define PMIX_ERR_UNKNOWN_DATA_TYPE                  -16
#define PMIX_ERR_TYPE_MISMATCH                      -18
#define PMIX_ERR_UNPACK_INADEQUATE_SPACE            -19
#define PMIX_ERR_UNPACK_FAILURE                     -20
#define PMIX_ERR_PACK_FAILURE                       -21
#define PMIX_ERR_NO_PERMISSIONS                     -23
#define PMIX_ERR_TIMEOUT                            -24
#define PMIX_ERR_UNREACH                            -25
#define PMIX_ERR_BAD_PARAM                          -27
#define PMIX_ERR_RESOURCE_BUSY                      -28
#define PMIX_ERR_OUT_OF_RESOURCE                    -29
#define PMIX_ERR_INIT                               -31
#define PMIX_ERR_NOMEM                              -32
#define PMIX_ERR_NOT_FOUND                          -46
#define PMIX_ERR_NOT_SUPPORTED                      -47
#define PMIX_ERR_PARAM_VALUE_NOT_SUPPORTED          -59
#define PMIX_ERR_COMM_FAILURE                       -49
#define PMIX_ERR_UNPACK_READ_PAST_END_OF_BUFFER     -50
#define PMIX_ERR_CONFLICTING_CLEANUP_DIRECTIVES     -51
#define PMIX_ERR_PARTIAL_SUCCESS                    -52
#define PMIX_ERR_DUPLICATE_KEY                      -53
#define PMIX_ERR_EMPTY                              -60
#define PMIX_ERR_LOST_CONNECTION                    -61
#define PMIX_ERR_EXISTS_OUTSIDE_SCOPE               -62

/* Process set */
#define PMIX_PROCESS_SET_DEFINE                     -55
#define PMIX_PROCESS_SET_DELETE                     -56

/* Debugger ops */
#define PMIX_DEBUGGER_RELEASE                       -3      // replaced deprecated PMIX_ERR_DEBUGGER_RELEASE
#define PMIX_READY_FOR_DEBUG                        -58     // accompanied by PMIX_BREAKPOINT indicating where proc is waiting

/* query errors */
#define PMIX_QUERY_PARTIAL_SUCCESS                  -104

/* job control */
#define PMIX_JCTRL_CHECKPOINT                       -106    // monitored by client to trigger checkpoint operation
#define PMIX_JCTRL_CHECKPOINT_COMPLETE              -107    // sent by client and monitored by server to notify that requested
                                                            //     checkpoint operation has completed
#define PMIX_JCTRL_PREEMPT_ALERT                    -108    // monitored by client to detect RM intends to preempt

/* monitoring */
#define PMIX_MONITOR_HEARTBEAT_ALERT                -109
#define PMIX_MONITOR_FILE_ALERT                     -110
#define PMIX_PROC_TERMINATED                        -111

/* operational */
#define PMIX_ERR_EVENT_REGISTRATION                 -144
#define PMIX_MODEL_DECLARED                         -147
#define PMIX_MODEL_RESOURCES                        -151     // model resource usage has changed
#define PMIX_OPENMP_PARALLEL_ENTERED                -152     // an OpenMP parallel region has been entered
#define PMIX_OPENMP_PARALLEL_EXITED                 -153     // an OpenMP parallel region has completed
#define PMIX_LAUNCHER_READY                         -155
#define PMIX_OPERATION_IN_PROGRESS                  -156
#define PMIX_OPERATION_SUCCEEDED                    -157
#define PMIX_ERR_INVALID_OPERATION                  -158
#define PMIX_GROUP_INVITED                          -159
#define PMIX_GROUP_LEFT                             -160
#define PMIX_GROUP_INVITE_ACCEPTED                  -161
#define PMIX_GROUP_INVITE_DECLINED                  -162
#define PMIX_GROUP_INVITE_FAILED                    -163
#define PMIX_GROUP_MEMBERSHIP_UPDATE                -164
#define PMIX_GROUP_CONSTRUCT_ABORT                  -165
#define PMIX_GROUP_CONSTRUCT_COMPLETE               -166
#define PMIX_GROUP_LEADER_SELECTED                  -167
#define PMIX_GROUP_LEADER_FAILED                    -168
#define PMIX_GROUP_CONTEXT_ID_ASSIGNED              -169
#define PMIX_GROUP_MEMBER_FAILED                    -170
#define PMIX_ERR_REPEAT_ATTR_REGISTRATION           -171
#define PMIX_ERR_IOF_FAILURE                        -172
#define PMIX_ERR_IOF_COMPLETE                       -173
#define PMIX_LAUNCH_COMPLETE                        -174     // include nspace of the launched job with notification
#define PMIX_FABRIC_UPDATED                         -175
#define PMIX_FABRIC_UPDATE_PENDING                  -176
#define PMIX_FABRIC_UPDATE_ENDPOINTS                -113

/* job-related errors */
#define PMIX_ERR_JOB_APP_NOT_EXECUTABLE             -177
#define PMIX_ERR_JOB_NO_EXE_SPECIFIED               -178
#define PMIX_ERR_JOB_FAILED_TO_MAP                  -179
#define PMIX_ERR_JOB_CANCELED                       -180
#define PMIX_ERR_JOB_FAILED_TO_LAUNCH               -181
#define PMIX_ERR_JOB_ABORTED                        -182
#define PMIX_ERR_JOB_KILLED_BY_CMD                  -183
#define PMIX_ERR_JOB_ABORTED_BY_SIG                 -184
#define PMIX_ERR_JOB_TERM_WO_SYNC                   -185
#define PMIX_ERR_JOB_SENSOR_BOUND_EXCEEDED          -186
#define PMIX_ERR_JOB_NON_ZERO_TERM                  -187
#define PMIX_ERR_JOB_ALLOC_FAILED                   -188
#define PMIX_ERR_JOB_ABORTED_BY_SYS_EVENT           -189
#define PMIX_ERR_JOB_EXE_NOT_FOUND                  -190
#define PMIX_ERR_JOB_WDIR_NOT_FOUND                 -233
#define PMIX_ERR_JOB_INSUFFICIENT_RESOURCES         -234
#define PMIX_ERR_JOB_SYS_OP_FAILED                  -235

/* job/session-related non-error events */
#define PMIX_EVENT_JOB_START                        -191
#define PMIX_EVENT_JOB_END                          -145
#define PMIX_EVENT_SESSION_START                    -192
#define PMIX_EVENT_SESSION_END                      -193

/* process-related events */
#define PMIX_ERR_PROC_REQUESTED_ABORT               -8
#define PMIX_ERR_PROC_TERM_WO_SYNC                  -200
#define PMIX_EVENT_PROC_TERMINATED                  -201
#define PMIX_ERR_PROC_KILLED_BY_CMD                 -400
#define PMIX_ERR_PROC_FAILED_TO_START               -401
#define PMIX_ERR_PROC_ABORTED_BY_SIG                -402
#define PMIX_ERR_PROC_SENSOR_BOUND_EXCEEDED         -403
#define PMIX_ERR_EXIT_NONZERO_TERM                  -404

/* system failures */
#define PMIX_EVENT_SYS_BASE                         -230
#define PMIX_EVENT_NODE_DOWN                        -231
#define PMIX_EVENT_NODE_OFFLINE                     -232
#define PMIX_EVENT_SYS_OTHER                        -330


/* define a macro for identifying system event values */
#define PMIX_SYSTEM_EVENT(a)   \
    ((a) <= PMIX_EVENT_SYS_BASE && PMIX_EVENT_SYS_OTHER <= (a))

/* used by event handlers */
#define PMIX_EVENT_NO_ACTION_TAKEN                  -331
#define PMIX_EVENT_PARTIAL_ACTION_TAKEN             -332
#define PMIX_EVENT_ACTION_DEFERRED                  -333
#define PMIX_EVENT_ACTION_COMPLETE                  -334

/* define a starting point for user-level defined error
 * constants - negative values larger than this are guaranteed
 * not to conflict with PMIx values. Definitions should always
 * be based on the PMIX_EXTERNAL_ERR_BASE constant and -not- a
 * specific value as the value of the constant may change */
#define PMIX_EXTERNAL_ERR_BASE                      -3000

/* define the results values for comparisons */
typedef enum {
    PMIX_EQUAL,
    PMIX_VALUE1_GREATER,
    PMIX_VALUE2_GREATER,
    PMIX_VALUE_TYPE_DIFFERENT,
    PMIX_VALUE_INCOMPATIBLE_OBJECTS,
    PMIX_VALUE_COMPARISON_NOT_AVAIL
} pmix_value_cmp_t;

/****    PMIX DATA TYPES    ****/
typedef uint16_t pmix_data_type_t;
#define PMIX_UNDEF                       0
#define PMIX_BOOL                        1  // converted to/from native true/false to uint8 for pack/unpack
#define PMIX_BYTE                        2  // a byte of data
#define PMIX_STRING                      3  // NULL-terminated string
#define PMIX_SIZE                        4  // size_t
#define PMIX_PID                         5  // OS-pid
#define PMIX_INT                         6
#define PMIX_INT8                        7
#define PMIX_INT16                       8
#define PMIX_INT32                       9
#define PMIX_INT64                      10
#define PMIX_UINT                       11
#define PMIX_UINT8                      12
#define PMIX_UINT16                     13
#define PMIX_UINT32                     14
#define PMIX_UINT64                     15
#define PMIX_FLOAT                      16
#define PMIX_DOUBLE                     17
#define PMIX_TIMEVAL                    18
#define PMIX_TIME                       19
#define PMIX_STATUS                     20  // needs to be tracked separately from integer for those times
                                            // when we are embedded and it needs to be converted to the
                                            // host error definitions
#define PMIX_VALUE                      21
#define PMIX_PROC                       22
#define PMIX_APP                        23
#define PMIX_INFO                       24
#define PMIX_PDATA                      25
// Hole left by deprecation/removal of PMIX_BUFFER
#define PMIX_BYTE_OBJECT                27
#define PMIX_KVAL                       28
// Hole left by deprecation/removal of PMIX_MODEX
#define PMIX_PERSIST                    30
#define PMIX_POINTER                    31
#define PMIX_SCOPE                      32
#define PMIX_DATA_RANGE                 33
#define PMIX_COMMAND                    34
#define PMIX_INFO_DIRECTIVES            35
#define PMIX_DATA_TYPE                  36
#define PMIX_PROC_STATE                 37
#define PMIX_PROC_INFO                  38
#define PMIX_DATA_ARRAY                 39
#define PMIX_PROC_RANK                  40
#define PMIX_QUERY                      41
#define PMIX_COMPRESSED_STRING          42  // string compressed with zlib
#define PMIX_ALLOC_DIRECTIVE            43
// Hole left by deprecation/removal of PMIX_INFO_ARRAY
#define PMIX_IOF_CHANNEL                45
#define PMIX_ENVAR                      46
#define PMIX_COORD                      47
#define PMIX_REGATTR                    48
#define PMIX_REGEX                      49
#define PMIX_JOB_STATE                  50
#define PMIX_LINK_STATE                 51
#define PMIX_PROC_CPUSET                52
#define PMIX_GEOMETRY                   53
#define PMIX_DEVICE_DIST                54
#define PMIX_ENDPOINT                   55
#define PMIX_TOPO                       56
#define PMIX_DEVTYPE                    57
#define PMIX_LOCTYPE                    58
#define PMIX_COMPRESSED_BYTE_OBJECT     59
#define PMIX_PROC_NSPACE                60
#define PMIX_PROC_STATS                 61
#define PMIX_DISK_STATS                 62
#define PMIX_NET_STATS                  63
#define PMIX_NODE_STATS                 64
#define PMIX_DATA_BUFFER                65
#define PMIX_STOR_MEDIUM                66
#define PMIX_STOR_ACCESS                67
#define PMIX_STOR_PERSIST               68
#define PMIX_STOR_ACCESS_TYPE           69
/********************/

/* define a boundary for implementers so they can add their own data types */
#define PMIX_DATA_TYPE_MAX     500


/* define a scope for data "put" by PMIx per the following:
 *
 * PMI_LOCAL - the data is intended only for other application
 *             processes on the same node. Data marked in this way
 *             will not be included in data packages sent to remote requestors
 * PMI_REMOTE - the data is intended solely for applications processes on
 *              remote nodes. Data marked in this way will not be shared with
 *              other processes on the same node
 * PMI_GLOBAL - the data is to be shared with all other requesting processes,
 *              regardless of location
 */
typedef uint8_t pmix_scope_t;
#define PMIX_SCOPE_UNDEF    0
#define PMIX_LOCAL          1   // share to procs also on this node
#define PMIX_REMOTE         2   // share with procs not on this node
#define PMIX_GLOBAL         3   // share with all procs (local + remote)
#define PMIX_INTERNAL       4   // store data in the internal tables

/* define a range for data "published" by PMIx
 */
typedef uint8_t pmix_data_range_t;
#define PMIX_RANGE_UNDEF        0
#define PMIX_RANGE_RM           1   // data is intended for the host resource manager
#define PMIX_RANGE_LOCAL        2   // available on local node only
#define PMIX_RANGE_NAMESPACE    3   // data is available to procs in the same nspace only
#define PMIX_RANGE_SESSION      4   // data available to all procs in session
#define PMIX_RANGE_GLOBAL       5   // data available to all procs
#define PMIX_RANGE_CUSTOM       6   // range is specified in a pmix_info_t
#define PMIX_RANGE_PROC_LOCAL   7   // restrict range to the local proc
#define PMIX_RANGE_INVALID   UINT8_MAX

/* define a "persistence" policy for data published by clients */
typedef uint8_t pmix_persistence_t;
#define PMIX_PERSIST_INDEF          0   // retain until specifically deleted
#define PMIX_PERSIST_FIRST_READ     1   // delete upon first access
#define PMIX_PERSIST_PROC           2   // retain until publishing process terminates
#define PMIX_PERSIST_APP            3   // retain until application terminates
#define PMIX_PERSIST_SESSION        4   // retain until session/allocation terminates
#define PMIX_PERSIST_INVALID   UINT8_MAX

/* define a set of bit-mask flags for specifying behavior of
 * command directives via pmix_info_t arrays */
typedef uint32_t pmix_info_directives_t;
#define PMIX_INFO_REQD              0x00000001
#define PMIX_INFO_ARRAY_END         0x00000002      // mark the end of an array created by PMIX_INFO_CREATE
#define PMIX_INFO_REQD_PROCESSED    0x00000004      // reqd attribute has been processed
#define PMIX_INFO_QUALIFIER         0x00000008      // info is a qualifier to the primary value
#define PMIX_INFO_PERSISTENT        0x00000010      // do not release included value
/* the top 16-bits are reserved for internal use by
 * implementers - these may be changed inside the
 * PMIx library */
#define PMIX_INFO_DIR_RESERVED 0xffff0000

/* define a set of directives for allocation requests */
typedef uint8_t pmix_alloc_directive_t;
#define PMIX_ALLOC_NEW          1  // new allocation is being requested. The resulting allocation will be
                                   // disjoint (i.e., not connected in a job sense) from the requesting allocation
#define PMIX_ALLOC_EXTEND       2  // extend the existing allocation, either in time or as additional resources
#define PMIX_ALLOC_RELEASE      3  // release part of the existing allocation. Attributes in the accompanying
                                   // pmix\_info\_t array may be used to specify permanent release of the
                                   // identified resources, or "lending" of those resources for some period
                                   // of time.
#define PMIX_ALLOC_REAQUIRE     4  // reacquire resources that were previously "lent" back to the scheduler

/* define a value boundary beyond which implementers are free
 * to define their own directive values */
#define PMIX_ALLOC_EXTERNAL     128


/* define a set of bit-mask flags for specifying IO
 * forwarding channels. These can be OR'd together
 * to reference multiple channels */
typedef uint16_t pmix_iof_channel_t;
#define PMIX_FWD_NO_CHANNELS        0x0000
#define PMIX_FWD_STDIN_CHANNEL      0x0001
#define PMIX_FWD_STDOUT_CHANNEL     0x0002
#define PMIX_FWD_STDERR_CHANNEL     0x0004
#define PMIX_FWD_STDDIAG_CHANNEL    0x0008
#define PMIX_FWD_ALL_CHANNELS       0x00ff

/* define values associated with PMIx_Group_join
 * to indicate accept and decline - this is
 * done for readability of user code */
typedef enum {
    PMIX_GROUP_DECLINE,
    PMIX_GROUP_ACCEPT
} pmix_group_opt_t;

typedef enum {
    PMIX_GROUP_CONSTRUCT,
    PMIX_GROUP_DESTRUCT
} pmix_group_operation_t;

/* define storage medium values
 * The pmix_storage_medium_t is a uint64_t type that defines
 * a set of bit-mask flags for specifying different types of
 * storage mediums. These can be bitwise OR'd together to
 * accommodate storage systems that mix storage medium types. */
typedef uint64_t pmix_storage_medium_t;
#define PMIX_STORAGE_MEDIUM_UNKNOWN     0x0000000000000001
#define PMIX_STORAGE_MEDIUM_TAPE        0x0000000000000002
#define PMIX_STORAGE_MEDIUM_HDD         0x0000000000000004
#define PMIX_STORAGE_MEDIUM_SSD         0x0000000000000008
#define PMIX_STORAGE_MEDIUM_NVME        0x0000000000000010
#define PMIX_STORAGE_MEDIUM_PMEM        0x0000000000000020
#define PMIX_STORAGE_MEDIUM_RAM         0x0000000000000040

/* define storage accessibility values
 * The pmix_storage_accessibility_t is a uint64_t type that
 * defines a set of bit-mask flags for specifying different
 * levels of storage accessibility (i.e,. from where a storage
 * system may be accessed). These can be bitwise OR'd together
 * to accommodate storage systems that are accessibile in
 * multiple ways. */
typedef uint64_t pmix_storage_accessibility_t;
#define PMIX_STORAGE_ACCESSIBILITY_NODE     0x0000000000000001
#define PMIX_STORAGE_ACCESSIBILITY_SESSION  0x0000000000000002
#define PMIX_STORAGE_ACCESSIBILITY_JOB      0x0000000000000004
#define PMIX_STORAGE_ACCESSIBILITY_RACK     0x0000000000000008
#define PMIX_STORAGE_ACCESSIBILITY_CLUSTER  0x0000000000000010
#define PMIX_STORAGE_ACCESSIBILITY_REMOTE   0x0000000000000020

/* define storage persistence values
 * The pmix_storage_persistence_t is a uint64_t type that defines
 * a set of bit-mask flags for specifying different levels of
 * persistence for a particular storage system. */
typedef uint64_t pmix_storage_persistence_t;
#define PMIX_STORAGE_PERSISTENCE_TEMPORARY  0x0000000000000001
#define PMIX_STORAGE_PERSISTENCE_NODE       0x0000000000000002
#define PMIX_STORAGE_PERSISTENCE_SESSION    0x0000000000000004
#define PMIX_STORAGE_PERSISTENCE_JOB        0x0000000000000008
#define PMIX_STORAGE_PERSISTENCE_SCRATCH    0x0000000000000010
#define PMIX_STORAGE_PERSISTENCE_PROJECT    0x0000000000000020
#define PMIX_STORAGE_PERSISTENCE_ARCHIVE    0x0000000000000040

/* define storage access values
 * The pmix_storage_access_type_t is a uint16_t type that defines
 * a set of bit-mask flags for specifying different storage system
 * access types. */
typedef uint16_t pmix_storage_access_type_t;
#define PMIX_STORAGE_ACCESS_RD      0x0001
#define PMIX_STORAGE_ACCESS_WR      0x0002
#define PMIX_STORAGE_ACCESS_RDWR    0x0003


/* define some "hooks" external libraries can use to
 * intercept memory allocation/release operations */
static inline void* pmix_malloc(size_t n)
{
    return malloc(n);
}

static inline void pmix_free(void *m)
{
    free(m);
}

static inline void* pmix_calloc(size_t n, size_t m)
{
    return calloc(n, m);
}

/* declare a convenience macro for checking keys */
#define PMIX_CHECK_KEY(a, b) \
    (0 == strncmp((a)->key, (b), PMIX_MAX_KEYLEN))

#define PMIX_CHECK_RESERVED_KEY(a) \
    (0 == strncmp((a), "pmix", 4))

#define PMIX_LOAD_KEY(a, b)                                                 \
    do {                                                                    \
        memset((a), 0, PMIX_MAX_KEYLEN+1);                                  \
        if (NULL != (b)) {                                                  \
            pmix_strncpy((char*)(a), (const char*)(b), PMIX_MAX_KEYLEN);    \
        }                                                                   \
    }while(0)

/* define a convenience macro for loading nspaces */
#define PMIX_LOAD_NSPACE(a, b)                              \
    do {                                                    \
        memset((a), 0, PMIX_MAX_NSLEN+1);                   \
        if (NULL != (b)) {                                  \
            pmix_strncpy((char*)(a), (b), PMIX_MAX_NSLEN);  \
        }                                                   \
    }while(0)

/* define a convenience macro for checking nspaces */
#define PMIX_CHECK_NSPACE(a, b) \
    (PMIX_NSPACE_INVALID((a)) || PMIX_NSPACE_INVALID((b)) || 0 == strncmp((a), (b), PMIX_MAX_NSLEN))

/* define a convenience macro for loading names */
#define PMIX_LOAD_PROCID(a, b, c)               \
    do {                                        \
        PMIX_LOAD_NSPACE((a)->nspace, (b));     \
        (a)->rank = (c);                        \
    }while(0)

#define PMIX_XFER_PROCID(a, b)      \
    memcpy((a), (b), sizeof(pmix_proc_t))

#define PMIX_PROCID_XFER(a, b) PMIX_XFER_PROCID(a, b)

/* define a convenience macro for checking names */
#define PMIX_CHECK_PROCID(a, b) \
    (PMIX_CHECK_NSPACE((a)->nspace, (b)->nspace) && ((a)->rank == (b)->rank || (PMIX_RANK_WILDCARD == (a)->rank || PMIX_RANK_WILDCARD == (b)->rank)))

#define PMIX_CHECK_RANK(a, b) \
    ((a) == (b) || (PMIX_RANK_WILDCARD == (a) || PMIX_RANK_WILDCARD == (b)))

#define PMIX_NSPACE_INVALID(a) \
    (NULL == (a) || 0 == pmix_nslen((a)))

#define PMIX_PROCID_INVALID(a)  \
    (PMIX_NSPACE_INVALID((a)->nspace) || PMIX_RANK_INVALID == (a)->rank)

/**
 * Provide a safe version of strncpy that doesn't generate
 * a ton of spurious warnings. Note that not every environment
 * provides nice string functions, and we aren't concerned about
 * max performance here
 *
 * @param dest Destination string.
 * @param src Source string.
 * @param len Size of the dest array - 1
 *
 */
static inline void pmix_strncpy(char *dest,
                                const char *src,
                                size_t len)
{
    size_t i;

    /* use an algorithm that also protects against
     * non-NULL-terminated src strings */
    for (i=0; i < len; ++i, ++src, ++dest) {
        *dest = *src;
        if ('\0' == *src) {
            break;
        }
    }
    *dest = '\0';
}

static inline size_t pmix_keylen(const char *src)
{
    size_t i, maxlen;

    if (NULL == src) {
        return 0;
    }
    maxlen = PMIX_MAX_KEYLEN + 1;
    /* use an algorithm that also protects against
     * non-NULL-terminated src strings */
    for (i=0; i < maxlen; ++i, ++src) {
        if ('\0' == *src) {
            break;
        }
    }
    return i;
}

static inline size_t pmix_nslen(const char *src)
{
    size_t i, maxlen;

    if (NULL == src) {
        return 0;
    }
    maxlen = PMIX_MAX_NSLEN + 1;
    /* use an algorithm that also protects against
     * non-NULL-terminated src strings */
    for (i=0; i < maxlen; ++i, ++src) {
        if ('\0' == *src) {
            break;
        }
    }
    return i;
}

static inline
int pmix_argv_count(char **argv)
{
    char **p;
    int i;

    if (NULL == argv)
        return 0;

    for (i = 0, p = argv; *p; i++, p++)
        continue;

    return i;
}

#define PMIX_ARGV_COUNT(r, a) \
    (r) = pmix_argv_count(a)

static inline
pmix_status_t pmix_argv_append_nosize(char ***argv, const char *arg)
{
    int argc;

    /* Create new argv. */

    if (NULL == *argv) {
        *argv = (char **) malloc(2 * sizeof(char *));
        if (NULL == *argv) {
            return PMIX_ERR_OUT_OF_RESOURCE;
        }
        argc = 0;
        (*argv)[0] = NULL;
        (*argv)[1] = NULL;
    }

    /* Extend existing argv. */
    else {
        /* count how many entries currently exist */
        argc = pmix_argv_count(*argv);

        *argv = (char **) realloc(*argv, (argc + 2) * sizeof(char *));
        if (NULL == *argv) {
            return PMIX_ERR_OUT_OF_RESOURCE;
        }
    }

    /* Set the newest element to point to a copy of the arg string */

    (*argv)[argc] = strdup(arg);
    if (NULL == (*argv)[argc]) {
        return PMIX_ERR_OUT_OF_RESOURCE;
    }

    argc = argc + 1;
    (*argv)[argc] = NULL;

    return PMIX_SUCCESS;
}

#define PMIX_ARGV_APPEND(r, a, b) \
    (r) = pmix_argv_append_nosize(&(a), (b))

static inline
pmix_status_t pmix_argv_prepend_nosize(char ***argv, const char *arg)
{
    int argc;
    int i;

    /* Create new argv. */

    if (NULL == *argv) {
        *argv = (char **) malloc(2 * sizeof(char *));
        if (NULL == *argv) {
            return PMIX_ERR_OUT_OF_RESOURCE;
        }
        (*argv)[0] = strdup(arg);
        (*argv)[1] = NULL;
    } else {
        /* count how many entries currently exist */
        argc = pmix_argv_count(*argv);

        *argv = (char **) realloc(*argv, (argc + 2) * sizeof(char *));
        if (NULL == *argv) {
            return PMIX_ERR_OUT_OF_RESOURCE;
        }
        (*argv)[argc + 1] = NULL;

        /* shift all existing elements down 1 */
        for (i = argc; 0 < i; i--) {
            (*argv)[i] = (*argv)[i - 1];
        }
        (*argv)[0] = strdup(arg);
    }

    return PMIX_SUCCESS;
}

#define PMIX_ARGV_PREPEND(r, a, b) \
    (r) = pmix_argv_prepend_nosize(&(a), b)

static inline
pmix_status_t pmix_argv_append_unique_nosize(char ***argv, const char *arg)
{
    int i;

    /* if the provided array is NULL, then the arg cannot be present,
     * so just go ahead and append
     */
    if (NULL == *argv) {
        return pmix_argv_append_nosize(argv, arg);
    }

    /* see if this arg is already present in the array */
    for (i = 0; NULL != (*argv)[i]; i++) {
        if (0 == strcmp(arg, (*argv)[i])) {
            /* already exists */
            return PMIX_SUCCESS;
        }
    }

    /* we get here if the arg is not in the array - so add it */
    return pmix_argv_append_nosize(argv, arg);
}

#define PMIX_ARGV_APPEND_UNIQUE(r, a, b) \
    (r) = pmix_argv_append_unique_nosize(a, b)

static inline void pmix_argv_free(char **argv)
{
    char **p;

    if (NULL == argv)
        return;

    for (p = argv; NULL != *p; ++p) {
        pmix_free(*p);
    }

    pmix_free(argv);
}

#define PMIX_ARGV_FREE(a)  pmix_argv_free(a)

static inline
char **pmix_argv_split_inter(const char *src_string,
                             int delimiter,
                             bool include_empty)
{
    char arg[512];
    char **argv = NULL;
    const char *p;
    char *argtemp;
    size_t arglen;

    while (src_string && *src_string) {
        p = src_string;
        arglen = 0;

        while (('\0' != *p) && (*p != delimiter)) {
            ++p;
            ++arglen;
        }

        /* zero length argument, skip */

        if (src_string == p) {
            if (include_empty) {
                arg[0] = '\0';
                if (PMIX_SUCCESS != pmix_argv_append_nosize(&argv, arg)) {
                    return NULL;
                }
            }
            src_string = p + 1;
            continue;
        }

        /* tail argument, add straight from the original string */

        else if ('\0' == *p) {
            if (PMIX_SUCCESS != pmix_argv_append_nosize(&argv, src_string)) {
                return NULL;
            }
            src_string = p;
            continue;
        }

        /* long argument, malloc buffer, copy and add */

        else if (arglen > 511) {
            argtemp = (char *) malloc(arglen + 1);
            if (NULL == argtemp)
                return NULL;

            pmix_strncpy(argtemp, src_string, arglen);
            argtemp[arglen] = '\0';

            if (PMIX_SUCCESS != pmix_argv_append_nosize(&argv, argtemp)) {
                free(argtemp);
                return NULL;
            }

            free(argtemp);
        }

        /* short argument, copy to buffer and add */

        else {
            pmix_strncpy(arg, src_string, arglen);
            arg[arglen] = '\0';

            if (PMIX_SUCCESS != pmix_argv_append_nosize(&argv, arg)) {
                return NULL;
            }
        }

        src_string = p + 1;
    }

    /* All done */

    return argv;
}

static inline
char **pmix_argv_split_with_empty(const char *src_string, int delimiter)
{
    return pmix_argv_split_inter(src_string, delimiter, true);
}

static inline
char **pmix_argv_split(const char *src_string, int delimiter)
{
    return pmix_argv_split_inter(src_string, delimiter, false);
}

#define PMIX_ARGV_SPLIT(a, b, c) \
    (a) = pmix_argv_split(b, c)

static inline
char *pmix_argv_join(char **argv, int delimiter)
{
    char **p;
    char *pp;
    char *str;
    size_t str_len = 0;
    size_t i;

    /* Bozo case */

    if (NULL == argv || NULL == argv[0]) {
        return strdup("");
    }

    /* Find the total string length in argv including delimiters.  The
     last delimiter is replaced by the NULL character. */

    for (p = argv; *p; ++p) {
        str_len += strlen(*p) + 1;
    }

    /* Allocate the string. */

    if (NULL == (str = (char *) malloc(str_len)))
        return NULL;

    /* Loop filling in the string. */

    str[--str_len] = '\0';
    p = argv;
    pp = *p;

    for (i = 0; i < str_len; ++i) {
        if ('\0' == *pp) {

            /* End of a string, fill in a delimiter and go to the next
             string. */

            str[i] = (char) delimiter;
            ++p;
            pp = *p;
        } else {
            str[i] = *pp++;
        }
    }

    /* All done */

    return str;
}

#define PMIX_ARGV_JOIN(a, b, c) \
    (a) = pmix_argv_join(b, c)

static inline
char **pmix_argv_copy(char **argv)
{
    char **dupv = NULL;

    if (NULL == argv)
        return NULL;

    /* create an "empty" list, so that we return something valid if we
     were passed a valid list with no contained elements */
    dupv = (char **) malloc(sizeof(char *));
    dupv[0] = NULL;

    while (NULL != *argv) {
        if (PMIX_SUCCESS != pmix_argv_append_nosize(&dupv, *argv)) {
            PMIX_ARGV_FREE(dupv);
            return NULL;
        }

        ++argv;
    }

    /* All done */

    return dupv;
}

#define PMIX_ARGV_COPY(a, b) \
    (a) = pmix_argv_copy(b)

/**
 * Portable version of setenv(3), allowing editing of any
 * environ-like array.
 *
 * @param name String name of the environment variable to look for
 * @param value String value to set (may be NULL)
 * @param overwrite Whether to overwrite any existing value with
 * the same name
 * @param env The environment to use
 *
 * @retval PMIX_ERR_OUT_OF_RESOURCE If internal malloc() fails.
 * @retval PMIX_ERR_EXISTS If the name already exists in \em env and
 * \em overwrite is false (and therefore the \em value was not
 * saved in \em env)
 * @retval PMIX_SUCESS If the value replaced another value or is
 * appended to \em env.
 *
 * \em env is expected to be a NULL-terminated array of pointers
 * (argv-style).  Note that unlike some implementations of
 * putenv(3), if \em value is inserted in \em env, it is copied.
 * So the caller can modify/free both \em name and \em value after
 * pmix_setenv() returns.
 *
 * The \em env array will be grown if necessary.
 *
 * It is permissible to invoke this function with the
 * system-defined \em environ variable.  For example:
 *
 * \code
 *   #include "pmix_common.h"
 *   pmix_setenv("foo", "bar", true, &environ);
 * \endcode
 *
 * NOTE: If you use the real environ, pmix_setenv() will turn
 * around and perform setenv() to put the value in the
 * environment.  This may very well lead to a memory leak, so its
 * use is strongly discouraged.
 *
 * It is also permissible to call this function with an empty \em
 * env, as long as it is pre-initialized with NULL:
 *
 * \code
 *   char **my_env = NULL;
 *   pmix_setenv("foo", "bar", true, &my_env);
 * \endcode
 */
static inline
pmix_status_t pmix_setenv(const char *name,
                          const char *value,
                          bool overwrite,
                          char ***env)
{
    int i;
    char newvalue[100000], compare[100000];
    size_t len;
    bool valid;

    /* Check the bozo case */
    if (NULL == env) {
        return PMIX_ERR_BAD_PARAM;
    }

    if (NULL != value) {
        /* check the string for unacceptable length - i.e., ensure
         * it is NULL-terminated */
        valid = false;
        for (i = 0; i < 100000; i++) {
            if ('\0' == value[i]) {
                valid = true;
                break;
            }
        }
        if (!valid) {
            return PMIX_ERR_BAD_PARAM;
        }
    }

    /* If this is the "environ" array, use setenv */
    if (*env == environ) {
        if (NULL == value) {
            /* this is actually an unsetenv request */
            unsetenv(name);
        } else {
            setenv(name, value, overwrite);
        }
        return PMIX_SUCCESS;
    }

    /* Make the new value */
    if (NULL == value) {
        snprintf(newvalue, 100000, "%s=", name);
    } else {
        snprintf(newvalue, 100000, "%s=%s", name, value);
    }

    if (NULL == *env) {
        pmix_argv_append_nosize(env, newvalue);
        return PMIX_SUCCESS;
    }

    /* Make something easy to compare to */

    snprintf(compare, 100000, "%s=", name);
    len = strlen(compare);

    /* Look for a duplicate that's already set in the env */

    for (i = 0; (*env)[i] != NULL; ++i) {
        if (0 == strncmp((*env)[i], compare, len)) {
            if (overwrite) {
                free((*env)[i]);
                (*env)[i] = strdup(newvalue);
                return PMIX_SUCCESS;
            } else {
                return PMIX_ERR_EXISTS;
            }
        }
    }

    /* If we found no match, append this value */

    pmix_argv_append_nosize(env, newvalue);

    /* All done */
    return PMIX_SUCCESS;
}

#define PMIX_SETENV(r, a, b, c) \
    (r) = pmix_setenv((a), (b), true, (c))


/****    PMIX COORD    ****/
/* define coordinate system views */
typedef uint8_t pmix_coord_view_t;
#define PMIX_COORD_VIEW_UNDEF       0x00
#define PMIX_COORD_LOGICAL_VIEW     0x01
#define PMIX_COORD_PHYSICAL_VIEW    0x02

/* define a structure for a proc's fabric coordinate */
typedef struct pmix_coord {
    pmix_coord_view_t view;
    uint32_t *coord;
    size_t dims;
} pmix_coord_t;

#define PMIX_COORD_STATIC_INIT      \
{                                   \
    .view = PMIX_COORD_VIEW_UNDEF,  \
    .coord = NULL,                  \
    .dims = 0                       \
}

#define PMIX_COORD_CREATE(m, d, n)                                              \
    do {                                                                        \
        pmix_coord_t *_m;                                                       \
        if (0 == (d)) {                                                         \
            (m) = NULL;                                                         \
        } else {                                                                \
            _m = (pmix_coord_t*)pmix_malloc((d) * sizeof(pmix_coord_t));        \
            if (NULL != _m) {                                                   \
                memset((m), 0, (d)*sizeof(pmix_coord_t));                       \
                _m->view = PMIX_COORD_VIEW_UNDEF;                               \
                _m->dims = (n);                                                 \
                if (0 == (n)) {                                                 \
                    _m->coord = NULL;                                           \
                } else {                                                        \
                    _m->coord = (uint32_t*)pmix_malloc((n) * sizeof(uint32_t)); \
                    if (NULL != _m->coord) {                                    \
                        memset(_m->coord, 0, (n)*sizeof(uint32_t));             \
                    }                                                           \
                }                                                               \
            }                                                                   \
            (m) = _m;                                                           \
        }                                                                       \
    } while(0)

#define PMIX_COORD_CONSTRUCT(m)             \
    do {                                    \
        (m)->view = PMIX_COORD_VIEW_UNDEF;  \
        (m)->coord = NULL;                  \
        (m)->dims = 0;                      \
    } while(0)

#define PMIX_COORD_DESTRUCT(m)              \
    do {                                    \
        (m)->view = PMIX_COORD_VIEW_UNDEF;  \
        if (NULL != (m)->coord) {           \
            pmix_free((m)->coord);          \
            (m)->coord = NULL;              \
            (m)->dims = 0;                  \
        }                                   \
    } while(0)

#define PMIX_COORD_FREE(m, n)                       \
    do {                                            \
        size_t _nc_;                                \
        if (NULL != (m)) {                          \
            for (_nc_ = 0; _nc_ < (n); _nc_++) {    \
                PMIX_COORD_DESTRUCT(&(m)[_nc_]);    \
            }                                       \
            free((m));                              \
            (m) = NULL;                             \
        }                                           \
    } while(0)


/****    PMIX LINK STATES    ****/
typedef uint8_t pmix_link_state_t;
#define PMIX_LINK_STATE_UNKNOWN     0  // The port state is unknown or not applicable
#define PMIX_LINK_DOWN              1  // The port is inactive.
#define PMIX_LINK_UP                2  // The port is active.


/****   PMIX CPUSET    ****/
typedef struct{
    char *source;
    void *bitmap;
} pmix_cpuset_t;

#define PMIX_CPUSET_STATIC_INIT \
{                               \
    .source = NULL,             \
    .bitmap = NULL              \
}

#define PMIX_CPUSET_CONSTRUCT(m) \
    memset((m), 0, sizeof(pmix_cpuset_t))

#define PMIX_CPUSET_CREATE(m, n)    \
    do {                                                                    \
        if (0 == (n))   {                                                   \
            (m) = NULL;                                                     \
        } else {                                                            \
            (m) = (pmix_cpuset_t*)pmix_malloc((n) * sizeof(pmix_cpuset_t)); \
            if (NULL != (m)) {                                              \
                memset((m), 0, (n) * sizeof(pmix_cpuset_t));                \
            }                                                               \
        }                                                                   \
    } while(0)


/****    PMIX BIND ENVELOPE    ****/
typedef uint8_t pmix_bind_envelope_t;
#define PMIX_CPUBIND_PROCESS    0
#define PMIX_CPUBIND_THREAD     1


/****    PMIX TOPOLOGY    ****/
typedef struct {
    char *source;
    void *topology;
} pmix_topology_t;

#define PMIX_TOPOLOGY_STATIC_INIT   \
{                                   \
    .source = NULL,                 \
    .topology = NULL                \
}

#define PMIX_TOPOLOGY_CONSTRUCT(m) \
    memset((m), 0, sizeof(pmix_topology_t))

#define PMIX_TOPOLOGY_CREATE(m, n) \
    do {                                                                        \
        if (0 == (n)) {                                                         \
            (m) = NULL;                                                         \
        } else {                                                                \
            (m) = (pmix_topology_t*)pmix_malloc((n) * sizeof(pmix_topology_t)); \
            if (NULL != (m)) {                                                  \
                memset((m), 0, (n) * sizeof(pmix_topology_t));                  \
            }                                                                   \
        }                                                                       \
    } while(0)

/**** PMIX RELATIVE LOCALITY    ****/
typedef uint16_t pmix_locality_t;
#define PMIX_LOCALITY_UNKNOWN           0x0000
#define PMIX_LOCALITY_NONLOCAL          0x8000
#define PMIX_LOCALITY_SHARE_HWTHREAD    0x0001
#define PMIX_LOCALITY_SHARE_CORE        0x0002
#define PMIX_LOCALITY_SHARE_L1CACHE     0x0004
#define PMIX_LOCALITY_SHARE_L2CACHE     0x0008
#define PMIX_LOCALITY_SHARE_L3CACHE     0x0010
#define PMIX_LOCALITY_SHARE_PACKAGE     0x0020
#define PMIX_LOCALITY_SHARE_NUMA        0x0040
#define PMIX_LOCALITY_SHARE_NODE        0x4000


/****    PMIX GEOMETRY     ****/
typedef struct pmix_geometry {
    size_t fabric;
    char *uuid;
    char *osname;
    pmix_coord_t *coordinates;
    size_t ncoords;
} pmix_geometry_t;

#define PMIX_GEOMETRY_STATIC_INIT   \
{                                   \
    .fabric = 0,                    \
    .uuid = NULL,                   \
    .osname = NULL,                 \
    .coordinates = NULL,            \
    .ncoords = 0                    \
}

#define PMIX_GEOMETRY_CONSTRUCT(m) \
    memset((m), 0, sizeof(pmix_geometry_t));

#define PMIX_GEOMETRY_DESTRUCT(m)                               \
    do {                                                        \
        if (NULL != (m)->uuid) {                                \
            free((m)->uuid);                                    \
            (m)->uuid = NULL;                                   \
        }                                                       \
        if (NULL != (m)->osname) {                              \
            free((m)->osname);                                  \
            (m)->osname = NULL;                                 \
        }                                                       \
        if (NULL != (m)->coordinates) {                         \
            PMIX_COORD_FREE((m)->coordinates, (m)->ncoords);    \
        }                                                       \
    } while(0)

#define PMIX_GEOMETRY_CREATE(m, n)                                              \
    do {                                                                        \
        if (0 == (n)) {                                                         \
            (m) = NULL;                                                         \
        } else {                                                                \
            (m) = (pmix_geometry_t*)pmix_malloc((n) * sizeof(pmix_geometry_t)); \
            if (NULL != (m)) {                                                  \
                memset((m), 0, (n) * sizeof(pmix_geometry_t));                  \
            }                                                                   \
        }                                                                       \
    } while(0)

#define PMIX_GEOMETRY_FREE(m, n)                    \
    do {                                            \
        size_t _i;                                  \
        if (NULL != (m)) {                          \
            for (_i=0; _i < (n); _i++) {            \
                PMIX_GEOMETRY_DESTRUCT(&(m)[_i]);   \
            }                                       \
            pmix_free((m));                         \
            (m) = NULL;                             \
        }                                           \
    } while(0)


/****    PMIX_DEVICE_TYPE    ****/
typedef uint64_t pmix_device_type_t;
#define PMIX_DEVTYPE_UNKNOWN        0x00
#define PMIX_DEVTYPE_BLOCK          0x01
#define PMIX_DEVTYPE_GPU            0x02
#define PMIX_DEVTYPE_NETWORK        0x04
#define PMIX_DEVTYPE_OPENFABRICS    0x08
#define PMIX_DEVTYPE_DMA            0x10
#define PMIX_DEVTYPE_COPROC         0x20

/****    PMIX_DISTANCE     ****/
typedef struct pmix_device_distance {
    char *uuid;
    char *osname;
    pmix_device_type_t type;
    uint16_t mindist;
    uint16_t maxdist;
} pmix_device_distance_t;

#define PMIX_DEVICE_DIST_STATIC_INIT    \
{                                       \
    .uuid = NULL,                       \
    .osname = NULL,                     \
    .type = PMIX_DEVTYPE_UNKNOWN,       \
    .mindist = 0,                       \
    .maxdist = 0                        \
}

#define PMIX_DEVICE_DIST_CONSTRUCT(m)                       \
    do {                                                    \
        memset((m), 0, sizeof(pmix_device_distance_t));     \
        (m)->mindist = UINT16_MAX;                          \
        (m)->maxdist = UINT16_MAX;                          \
    } while(0);

#define PMIX_DEVICE_DIST_DESTRUCT(m)    \
    do {                                \
        if (NULL != ((m)->uuid)) {      \
            pmix_free((m)->uuid);       \
        }                               \
        if (NULL != ((m)->osname)) {    \
            pmix_free((m)->osname);     \
        }                               \
    } while(0)

#define PMIX_DEVICE_DIST_CREATE(m, n)                                                           \
    do {                                                                                        \
        size_t _i;                                                                              \
        pmix_device_distance_t *_m;                                                             \
        if (0 == (n)) {                                                                         \
            (m) = NULL;                                                                         \
        } else {                                                                                \
            _m = (pmix_device_distance_t*)pmix_malloc((n) * sizeof(pmix_device_distance_t));    \
            if (NULL != _m) {                                                                   \
                memset(_m, 0, (n)*sizeof(pmix_device_distance_t));                              \
                for (_i=0; _i < (n); _i++) {                                                    \
                    _m[_i].mindist = UINT16_MAX;                                                \
                    _m[_i].maxdist = UINT16_MAX;                                                \
                }                                                                               \
            }                                                                                   \
            (m) = _m;                                                                           \
        }                                                                                       \
    } while(0)

#define PMIX_DEVICE_DIST_FREE(m, n)                     \
    do {                                                \
        size_t _i;                                      \
        if (NULL != (m)) {                              \
            for (_i=0; _i < (n); _i++) {                \
                PMIX_DEVICE_DIST_DESTRUCT(&(m)[_i]);    \
            }                                           \
            pmix_free((m));                             \
            (m) = NULL;                                 \
        }                                               \
    } while(0)


/****    PMIX BYTE OBJECT    ****/
typedef struct pmix_byte_object {
    char *bytes;
    size_t size;
} pmix_byte_object_t;

#define PMIX_BYTE_OBJECT_STATIC_INIT    \
{                                       \
    .bytes = NULL,                      \
    .size = 0                           \
}

#define PMIX_BYTE_OBJECT_CREATE(m, n)                                                   \
    do {                                                                                \
        if (0 == (n)) {                                                                 \
            (m) = NULL;                                                                 \
        } else {                                                                        \
            (m) = (pmix_byte_object_t*)pmix_malloc((n) * sizeof(pmix_byte_object_t));   \
            if (NULL != (m)) {                                                          \
                memset((m), 0, (n)*sizeof(pmix_byte_object_t));                         \
            }                                                                           \
        }                                                                               \
    } while(0)

#define PMIX_BYTE_OBJECT_CONSTRUCT(m)   \
    do {                                \
        (m)->bytes = NULL;              \
        (m)->size = 0;                  \
    } while(0)

#define PMIX_BYTE_OBJECT_DESTRUCT(m)    \
    do {                                \
        if (NULL != (m)->bytes) {       \
            pmix_free((m)->bytes);      \
        }                               \
        (m)->bytes = NULL;              \
        (m)->size = 0;                  \
    } while(0)

#define PMIX_BYTE_OBJECT_FREE(m, n)                     \
    do {                                                \
        size_t _bon;                                    \
        if (NULL != (m)) {                              \
            for (_bon=0; _bon < n; _bon++) {            \
                PMIX_BYTE_OBJECT_DESTRUCT(&(m)[_bon]);  \
            }                                           \
            pmix_free((m));                             \
            (m) = NULL;                                 \
        }                                               \
    } while(0)

#define PMIX_BYTE_OBJECT_LOAD(b, d, s)      \
    do {                                    \
        (b)->bytes = (char*)(d);            \
        (d) = NULL;                         \
        (b)->size = (s);                    \
        (s) = 0;                            \
    } while(0)


/****    PMIX ENDPOINT    ****/
typedef struct pmix_endpoint {
    char *uuid;
    char *osname;
    pmix_byte_object_t endpt;
} pmix_endpoint_t;

#define PMIX_ENDPOINT_STATIC_INIT           \
{                                           \
    .uuid = NULL,                           \
    .osname = NULL,                         \
    .endpt = PMIX_BYTE_OBJECT_STATIC_INIT   \
}

#define PMIX_ENDPOINT_CONSTRUCT(m)      \
    memset((m), 0, sizeof(pmix_endpoint_t))

#define PMIX_ENDPOINT_DESTRUCT(m)       \
    do {                                \
        if (NULL != (m)->uuid) {        \
            free((m)->uuid);            \
        }                               \
        if (NULL != (m)->osname) {      \
            free((m)->osname);          \
        }                               \
        if (NULL != (m)->endpt.bytes) { \
            free((m)->endpt.bytes);     \
        }                               \
    } while(0)

#define PMIX_ENDPOINT_CREATE(m, n)                                              \
    do {                                                                        \
        if (0 == (n)) {                                                         \
            (m) = NULL;                                                         \
        } else {                                                                \
            (m) = (pmix_endpoint_t*)pmix_malloc((n) * sizeof(pmix_endpoint_t)); \
            if (NULL != (m)) {                                                  \
                memset((m), 0, (n) * sizeof(pmix_endpoint_t));                  \
            }                                                                   \
        }                                                                       \
    } while(0)

#define PMIX_ENDPOINT_FREE(m, n)                    \
    do {                                            \
        size_t _n;                                  \
        if (NULL != (m)) {                          \
            for (_n=0; _n < (n); _n++) {            \
                PMIX_ENDPOINT_DESTRUCT(&((m)[_n])); \
            }                                       \
            free((m));                              \
            (m) = NULL;                             \
        }                                           \
    } while(0)



/****    PMIX ENVAR STRUCT   ****/
/* Provide a structure for specifying environment variable modifications
 * Standard environment variables (e.g., PATH, LD_LIBRARY_PATH, and LD_PRELOAD)
 * take multiple arguments separated by delimiters. Unfortunately, the delimiters
 * depend upon the variable itself - some use semi-colons, some colons, etc. Thus,
 * the operation requires not only the name of the variable to be modified and
 * the value to be inserted, but also the separator to be used when composing
 * the aggregate value
 */
typedef struct {
    char *envar;
    char *value;
    char separator;
} pmix_envar_t;

#define PMIX_ENVAR_STATIC_INIT  \
{                               \
    .envar = NULL,              \
    .value = NULL,              \
    .separator = '\0'           \
}

#define PMIX_ENVAR_CREATE(m, n)                                             \
    do {                                                                    \
        if (0 == (n)) {                                                     \
            (m) = NULL;                                                     \
        } else {                                                            \
            (m) = (pmix_envar_t*)pmix_malloc((n) * sizeof(pmix_envar_t));   \
            if (NULL != (m)) {                                              \
                memset((m), 0, (n) * sizeof(pmix_envar_t));                 \
            }                                                               \
        }                                                                   \
    } while (0)
#define PMIX_ENVAR_FREE(m, n)                       \
    do {                                            \
        size_t _ek;                                 \
        if (NULL != (m)) {                          \
            for (_ek=0; _ek < (n); _ek++) {         \
               PMIX_ENVAR_DESTRUCT(&(m)[_ek]);      \
            }                                       \
            pmix_free((m));                         \
        }                                           \
    } while (0)
#define PMIX_ENVAR_CONSTRUCT(m)        \
    do {                               \
        (m)->envar = NULL;             \
        (m)->value = NULL;             \
        (m)->separator = '\0';         \
    } while(0)
#define PMIX_ENVAR_DESTRUCT(m)         \
    do {                               \
        if (NULL != (m)->envar) {      \
            pmix_free((m)->envar);     \
            (m)->envar = NULL;         \
        }                              \
        if (NULL != (m)->value) {      \
            pmix_free((m)->value);      \
            (m)->value = NULL;         \
        }                              \
    } while(0)
#define PMIX_ENVAR_LOAD(m, e, v, s)    \
    do {                               \
        if (NULL != (e)) {             \
            (m)->envar = strdup(e);    \
        }                              \
        if (NULL != (v)) {             \
            (m)->value = strdup(v);    \
        }                              \
        (m)->separator = (s);          \
    } while(0)


/****    PMIX DATA BUFFER MACROS   ****/
#define PMIX_DATA_BUFFER_STATIC_INIT    \
{                                       \
    .base_ptr = NULL,                   \
    .pack_ptr = NULL,                   \
    .unpack_ptr = NULL,                 \
    .bytes_allocated = 0,               \
    .bytes_used = 0                     \
}
#define PMIX_DATA_BUFFER_CREATE(m)                                          \
    do {                                                                    \
        (m) = (pmix_data_buffer_t*)pmix_malloc(sizeof(pmix_data_buffer_t)); \
        if (NULL != (m)) {                                                  \
            memset((m), 0, sizeof(pmix_data_buffer_t));                     \
        }                                                                   \
    } while (0)
#define PMIX_DATA_BUFFER_RELEASE(m)             \
    do {                                        \
        if (NULL != (m)->base_ptr) {            \
            pmix_free((m)->base_ptr);           \
        }                                       \
        pmix_free((m));                         \
        (m) = NULL;                             \
    } while (0)
#define PMIX_DATA_BUFFER_CONSTRUCT(m)       \
    memset((m), 0, sizeof(pmix_data_buffer_t))
#define PMIX_DATA_BUFFER_DESTRUCT(m)        \
    do {                                    \
        if (NULL != (m)->base_ptr) {        \
            pmix_free((m)->base_ptr);       \
            (m)->base_ptr = NULL;           \
        }                                   \
        (m)->pack_ptr = NULL;               \
        (m)->unpack_ptr = NULL;             \
        (m)->bytes_allocated = 0;           \
        (m)->bytes_used = 0;                \
    } while (0)
#define PMIX_DATA_BUFFER_LOAD(b, d, s)  \
    do {                                \
        pmix_byte_object_t _bo;         \
        _bo.bytes = (char*)(d);         \
        _bo.size = (s);                 \
        PMIx_Data_load((b), &_bo);      \
    } while(0)

#define PMIX_DATA_BUFFER_UNLOAD(b, d, s)    \
    do {                                    \
        pmix_byte_object_t _bo;             \
        pmix_status_t _r;                   \
        _r = PMIx_Data_unload((b), &_bo);   \
        if (PMIX_SUCCESS == _r) {           \
            (d) = _bo.bytes;                \
            (s) = _bo.size;                 \
        } else {                            \
            (d) = NULL;                     \
            (s) = 0;                        \
        }                                   \
    } while(0)

/****    PMIX PROC OBJECT    ****/
typedef struct pmix_proc {
    pmix_nspace_t nspace;
    pmix_rank_t rank;
} pmix_proc_t;

#define PMIX_PROC_STATIC_INIT   \
{                               \
    .nspace = {0},              \
    .rank = PMIX_RANK_UNDEF     \
}

#define PMIX_PROC_CREATE(m, n)                                          \
    do {                                                                \
        if (0 == (n)) {                                                 \
            (m) = NULL;                                                 \
        } else {                                                        \
            (m) = (pmix_proc_t*)pmix_malloc((n) * sizeof(pmix_proc_t)); \
            if (NULL != (m)) {                                          \
                memset((m), 0, (n) * sizeof(pmix_proc_t));              \
            }                                                           \
        }                                                               \
    } while (0)

#define PMIX_PROC_RELEASE(m)    \
    do {                        \
        pmix_free((m));         \
        (m) = NULL;             \
    } while (0)

#define PMIX_PROC_CONSTRUCT(m)                  \
    do {                                        \
        memset((m), 0, sizeof(pmix_proc_t));    \
    } while (0)

#define PMIX_PROC_DESTRUCT(m)

#define PMIX_PROC_FREE(m, n)                    \
    do {                                        \
        if (NULL != (m)) {                      \
            pmix_free((m));                     \
            (m) = NULL;                         \
        }                                       \
    } while (0)

#define PMIX_PROC_LOAD(m, n, r)                                 \
    do {                                                        \
        PMIX_PROC_CONSTRUCT((m));                               \
        pmix_strncpy((char*)(m)->nspace, (n), PMIX_MAX_NSLEN);  \
        (m)->rank = (r);                                        \
    } while(0)

#define PMIX_MULTICLUSTER_NSPACE_CONSTRUCT(t, c, n)                         \
    do {                                                                    \
        size_t _len;                                                        \
        memset((t), 0, PMIX_MAX_NSLEN+1);                                   \
        _len = pmix_nslen((c));                                             \
        if ((_len + pmix_nslen((n))) < PMIX_MAX_NSLEN) {                    \
            pmix_strncpy((char*)(t), (c), PMIX_MAX_NSLEN);                  \
            (t)[_len] = ':';                                                \
            pmix_strncpy((char*)&(t)[_len+1], (n), PMIX_MAX_NSLEN - _len);  \
        }                                                                   \
    } while(0)

#define PMIX_MULTICLUSTER_NSPACE_PARSE(t, c, n)             \
    do {                                                    \
        size_t _n, _j;                                      \
        for (_n=0; '\0' != (t)[_n] && ':' != (t)[_n] &&     \
             _n <= PMIX_MAX_NSLEN; _n++) {                  \
            (c)[_n] = (t)[_n];                              \
        }                                                   \
        _n++;                                               \
        for (_j=0; _n <= PMIX_MAX_NSLEN &&                  \
             '\0' != (t)[_n]; _n++, _j++) {                 \
            (n)[_j] = (t)[_n];                              \
        }                                                   \
    } while(0)


/****    PMIX PROC INFO STRUCT    ****/
typedef struct pmix_proc_info {
    pmix_proc_t proc;
    char *hostname;
    char *executable_name;
    pid_t pid;
    int exit_code;
    pmix_proc_state_t state;
} pmix_proc_info_t;

#define PMIX_PROC_INFO_STATIC_INIT  \
{                                   \
    .proc = PMIX_PROC_STATIC_INIT,  \
    .hostname = NULL,               \
    .executable_name = NULL,        \
    .pid = 0,                       \
    .exit_code = 0,                 \
    .state = PMIX_PROC_STATE_UNDEF  \
}

#define PMIX_PROC_INFO_CREATE(m, n)                                                 \
    do {                                                                            \
        if (0 == (n)) {                                                             \
            (m) = NULL;                                                             \
        } else {                                                                    \
            (m) = (pmix_proc_info_t*)pmix_malloc((n) * sizeof(pmix_proc_info_t));   \
            if (NULL != (m)) {                                                      \
                memset((m), 0, (n) * sizeof(pmix_proc_info_t));                     \
            }                                                                       \
        }                                                                           \
    } while (0)

#define PMIX_PROC_INFO_RELEASE(m)      \
    do {                               \
        PMIX_PROC_INFO_FREE((m), 1);   \
    } while (0)

#define PMIX_PROC_INFO_CONSTRUCT(m)                 \
    do {                                            \
        memset((m), 0, sizeof(pmix_proc_info_t));   \
    } while (0)

#define PMIX_PROC_INFO_DESTRUCT(m)              \
    do {                                        \
        if (NULL != (m)->hostname) {            \
            pmix_free((m)->hostname);           \
            (m)->hostname = NULL;               \
        }                                       \
        if (NULL != (m)->executable_name) {     \
            pmix_free((m)->executable_name);    \
            (m)->executable_name = NULL;        \
        }                                       \
    } while(0)

#define PMIX_PROC_INFO_FREE(m, n)                   \
    do {                                            \
        size_t _k;                                  \
        if (NULL != (m)) {                          \
            for (_k=0; _k < (n); _k++) {            \
                PMIX_PROC_INFO_DESTRUCT(&(m)[_k]);  \
            }                                       \
            pmix_free((m));                         \
        }                                           \
    } while (0)


/****    PMIX DATA ARRAY STRUCT    ****/

typedef struct pmix_data_array {
    pmix_data_type_t type;
    size_t size;
    void *array;
} pmix_data_array_t;

#define PMIX_DATA_ARRAY_STATIC_INIT     \
{                                       \
    .type = PMIX_UNDEF,                 \
    .size = 0,                          \
    .array = NULL                       \
}

/**** THE PMIX_DATA_ARRAY SUPPORT MACROS ARE DEFINED ****/
/**** DOWN BELOW (NEAR THE BOTTOM OF THE FILE) TO    ****/
/**** AVOID CIRCULAR DEPENDENCIES                    ****/


/* we cannot forward-declare the pmix_regattr_t struct
 * as Cython doesn't know what to do with it. Thus, we
 * will utilize the void* entry of the pmix_value_t to
 * hold the pointer to pmix_regattr_t */

/****    PMIX DATA BUFFER    ****/
typedef struct pmix_data_buffer {
    /** Start of my memory */
    char *base_ptr;
    /** Where the next data will be packed to (within the allocated
        memory starting at base_ptr) */
    char *pack_ptr;
    /** Where the next data will be unpacked from (within the
        allocated memory starting as base_ptr) */
    char *unpack_ptr;
    /** Number of bytes allocated (starting at base_ptr) */
    size_t bytes_allocated;
    /** Number of bytes used by the buffer (i.e., amount of data --
        including overhead -- packed in the buffer) */
    size_t bytes_used;
} pmix_data_buffer_t;

#define PMIX_DATA_BUFFER_STATIC_INIT    \
{                                       \
    .base_ptr = NULL,                   \
    .pack_ptr = NULL,                   \
    .unpack_ptr = NULL,                 \
    .bytes_allocated = 0,               \
    .bytes_used = 0                     \
}

/****   STATISTICS STRUCTURES  ****/
typedef struct pmix_proc_stats {
    /* process ident info */
    char *node;
    pmix_proc_t proc;
    pid_t pid;
    char *cmd;
    /* process stats */
    char state;
    struct timeval time;
    float percent_cpu;
    int32_t priority;
    uint16_t num_threads;
    float pss;   /* in MBytes */
    float vsize;  /* in MBytes */
    float rss;  /* in MBytes */
    float peak_vsize;  /* in MBytes */
    uint16_t processor;
    /* time at which sample was taken */
    struct timeval sample_time;
} pmix_proc_stats_t;

#define PMIX_PROC_STATS_STATIC_INIT     \
{                                       \
    .node = NULL,                       \
    .proc = PMIX_PROC_STATIC_INIT,      \
    .pid = 0,                           \
    .cmd = NULL,                        \
    .state = '\0',                      \
    .time = {0, 0},                     \
    .percent_cpu = 0.0,                 \
    .priority = 0,                      \
    .num_threads = 0,                   \
    .pss = 0.0,                         \
    .vsize = 0.0,                       \
    .rss = 0.0,                         \
    .peak_vsize = 0.0                   \
    .processor = 0,                     \
    .sample_time = {0, 0}               \
}

#define PMIX_PROC_STATS_CREATE(m, n)                                                \
    do {                                                                            \
        if (0 == (n)) {                                                             \
            (m) = NULL;                                                             \
        } else {                                                                    \
            (m) = (pmix_proc_stats_t*)pmix_malloc((n) * sizeof(pmix_proc_stats_t)); \
            if (NULL != (m)) {                                                      \
                memset((m), 0, (n) * sizeof(pmix_proc_stats_t));                    \
            }                                                                       \
        }                                                                           \
    } while (0)

#define PMIX_PROC_STATS_RELEASE(m)      \
    do {                                \
        PMIX_PROC_STATS_FREE((m), 1);   \
    } while (0)

#define PMIX_PROC_STATS_CONSTRUCT(m)                \
    do {                                            \
        memset((m), 0, sizeof(pmix_proc_stats_t));  \
    } while (0)

#define PMIX_PROC_STATS_DESTRUCT(m)     \
    do {                                \
        if (NULL != (m)->node) {        \
            pmix_free((m)->node);       \
            (m)->node = NULL;           \
        }                               \
        if (NULL != (m)->cmd) {         \
            pmix_free((m)->cmd);        \
            (m)->cmd = NULL;            \
        }                               \
    } while(0)

static inline void pmix_proc_stats_free(pmix_proc_stats_t *ps, size_t n)
{
    size_t k;

    if (NULL != ps) {
        for (k=0; k < n; k++) {
            PMIX_PROC_STATS_DESTRUCT(&ps[k]);
        }
    }
}

#define PMIX_PROC_STATS_FREE(m, n)  \
do {                                \
    pmix_proc_stats_free(m, n);     \
    pmix_free(m);                   \
    (m) = NULL;                     \
} while(0)

typedef struct {
    char *disk;
    uint64_t num_reads_completed;
    uint64_t num_reads_merged;
    uint64_t num_sectors_read;
    uint64_t milliseconds_reading;
    uint64_t num_writes_completed;
    uint64_t num_writes_merged;
    uint64_t num_sectors_written;
    uint64_t milliseconds_writing;
    uint64_t num_ios_in_progress;
    uint64_t milliseconds_io;
    uint64_t weighted_milliseconds_io;
} pmix_disk_stats_t;

#define PMIX_DISK_STATS_STATIC_INIT     \
{                                       \
    .disk = NULL,                       \
    .num_reads_completed = 0,           \
    .num_reads_merged = 0,              \
    .num_sectors_read = 0,              \
    .milliseconds_reading = 0,          \
    .num_writes_completed = 0,          \
    .num_writes_merged = 0,             \
    .num_sectors_written = 0,           \
    .milliseconds_writing = 0,          \
    .num_ios_in_progress = 0,           \
    .milliseconds_io = 0,               \
    .weighted_milliseconds_io = 0       \
}

#define PMIX_DISK_STATS_CREATE(m, n)                                                \
    do {                                                                            \
        if (0 == (n)) {                                                             \
            (m) = NULL;                                                             \
        } else {                                                                    \
            (m) = (pmix_disk_stats_t*)pmix_malloc((n) * sizeof(pmix_disk_stats_t)); \
            if (NULL != (m)) {                                                      \
                memset((m), 0, (n) * sizeof(pmix_disk_stats_t));                    \
            }                                                                       \
        }                                                                           \
    } while (0)

#define PMIX_DISK_STATS_RELEASE(m)      \
    do {                                \
        PMIX_DISK_STATS_FREE((m), 1);   \
    } while (0)

#define PMIX_DISK_STATS_CONSTRUCT(m)                \
    do {                                            \
        memset((m), 0, sizeof(pmix_disk_stats_t));  \
    } while (0)

#define PMIX_DISK_STATS_DESTRUCT(m)     \
    do {                                \
        if (NULL != (m)->disk) {        \
            pmix_free((m)->disk);       \
            (m)->disk = NULL;           \
        }                               \
    } while(0)

static inline void pmix_disk_stats_free(pmix_disk_stats_t *d, size_t n)
{
    size_t k;

    if (NULL != d) {
        for (k=0; k < n; k++) {
            PMIX_DISK_STATS_DESTRUCT(&d[k]);
        }
    }
}

#define PMIX_DISK_STATS_FREE(m, n)  \
do {                                \
    pmix_disk_stats_free(m, n);     \
    pmix_free(m);                   \
    (m) = NULL;                     \
} while(0)

typedef struct {
    char *net_interface;
    uint64_t num_bytes_recvd;
    uint64_t num_packets_recvd;
    uint64_t num_recv_errs;
    uint64_t num_bytes_sent;
    uint64_t num_packets_sent;
    uint64_t num_send_errs;
} pmix_net_stats_t;

#define PMIX_NET_STATS_STATIC_INIT  \
{                                   \
    .net_interface = NULL,          \
    .num_bytes_recvd = 0,           \
    .num_packets_recvd = 0,         \
    .num_recv_errs = 0,             \
    .num_bytes_sent = 0,            \
    .num_packets_sent = 0,          \
    .num_send_errs = 0              \
}

#define PMIX_NET_STATS_CREATE(m, n)                                                 \
    do {                                                                            \
        if (0 == (n)) {                                                             \
            (m) = NULL;                                                             \
        } else {                                                                    \
            (m) = (pmix_net_stats_t*)pmix_malloc((n) * sizeof(pmix_net_stats_t));   \
            if (NULL != (m)) {                                                      \
                memset((m), 0, (n) * sizeof(pmix_net_stats_t));                     \
            }                                                                       \
        }                                                                           \
    } while (0)

#define PMIX_NET_STATS_RELEASE(m)       \
    do {                                \
        PMIX_NET_STATS_FREE((m), 1);    \
    } while (0)

#define PMIX_NET_STATS_CONSTRUCT(m)                 \
    do {                                            \
        memset((m), 0, sizeof(pmix_net_stats_t));   \
    } while (0)

#define PMIX_NET_STATS_DESTRUCT(m)          \
    do {                                    \
        if (NULL != (m)->net_interface) {   \
            pmix_free((m)->net_interface);  \
            (m)->net_interface = NULL;      \
        }                                   \
    } while(0)

static inline void pmix_net_stats_free(pmix_net_stats_t *nst, size_t n)
{
    size_t k;

    if (NULL != nst) {
        for (k=0; k < n; k++) {
            PMIX_NET_STATS_DESTRUCT(&nst[k]);
        }
    }
}

#define PMIX_NET_STATS_FREE(m, n)   \
do {                                \
    pmix_net_stats_free(m, n);      \
    pmix_free(m);                   \
    (m) = NULL;                     \
} while(0)

typedef struct {
    char *node;
    /* node-level load averages */
    float la;
    float la5;
    float la15;
    /* memory usage */
    float total_mem;  /* in MBytes */
    float free_mem;  /* in MBytes */
    float buffers;  /* in MBytes */
    float cached;   /* in MBytes */
    float swap_cached;  /* in MBytes */
    float swap_total;   /* in MBytes */
    float swap_free;    /* in MBytes */
    float mapped;       /* in MBytes */
    /* time at which sample was taken */
    struct timeval sample_time;
    /* array of disk stats, one per disk */
    pmix_disk_stats_t *diskstats;
    size_t ndiskstats;
    /* array of net stats, one per interface */
    pmix_net_stats_t *netstats;
    size_t nnetstats;
} pmix_node_stats_t;

#define PMIX_NODE_STATS_STATIC_INIT     \
{                                       \
    .node = NULL,                       \
    .la = 0,                            \
    .la5 = 0,                           \
    .la15 = 0,                          \
    .total_mem = 0.0,                   \
    .free_mem = 0.0,                    \
    .buffers = 0.0,                     \
    .cached = 0.0,                      \
    .swap_cached = 0.0,                 \
    .swap_total = 0.0,                  \
    .swap_free = 0.0,                   \
    .mapped = 0.0,                      \
    .sample_time = {0, 0},              \
    .diskstats = NULL,                  \
    .ndiskstats = 0,                    \
    .netstats = NULL,                   \
    .nnetstats = 0                      \
}

#define PMIX_NODE_STATS_CREATE(m, n)                                                \
    do {                                                                            \
        if (0 == (n)) {                                                             \
            (m) = NULL;                                                             \
        } else {                                                                    \
            (m) = (pmix_node_stats_t*)pmix_malloc((n) * sizeof(pmix_node_stats_t)); \
            if (NULL != (m)) {                                                      \
                memset((m), 0, (n) * sizeof(pmix_node_stats_t));                    \
            }                                                                       \
        }                                                                           \
    } while (0)

#define PMIX_NODE_STATS_CONSTRUCT(m)                \
    do {                                            \
        memset((m), 0, sizeof(pmix_node_stats_t));  \
    } while (0)

#define PMIX_NODE_STATS_DESTRUCT(m)                                 \
    do {                                                            \
        if (NULL != (m)->node) {                                    \
            pmix_free((m)->node);                                   \
            (m)->node = NULL;                                       \
        }                                                           \
        if (NULL != (m)->diskstats) {                               \
            PMIX_DISK_STATS_FREE((m)->diskstats, (m)->ndiskstats);  \
        }                                                           \
        if (NULL != (m)->netstats) {                                \
            PMIX_NET_STATS_FREE((m)->netstats, (m)->nnetstats);     \
        }                                                           \
    } while(0)

static inline void pmix_node_stats_free(pmix_node_stats_t *nd, size_t n)
{
    size_t k;

    if (NULL != nd) {
        for (k=0; k < n; k++) {
            PMIX_NODE_STATS_DESTRUCT(&nd[k]);
        }
    }
}

#define PMIX_NODE_STATS_FREE(m, n)  \
do {                                \
    pmix_node_stats_free(m, n);     \
    pmix_free(m);                   \
    (m) = NULL;                     \
} while(0)

#define PMIX_NODE_STATS_RELEASE(m)  \
    pmix_node_stats_free(m, 1)


/****    PMIX VALUE STRUCT    ****/

/* NOTE: operations can supply a collection of values under
 * a single key by passing a pmix_value_t containing a
 * data array of type PMIX_INFO, with each array element
 * containing its own pmix_info_t object */

typedef struct pmix_value {
    pmix_data_type_t type;
    union {
        bool flag;
        uint8_t byte;
        char *string;
        size_t size;
        pid_t pid;
        int integer;
        int8_t int8;
        int16_t int16;
        int32_t int32;
        int64_t int64;
        unsigned int uint;
        uint8_t uint8;
        uint16_t uint16;
        uint32_t uint32;
        uint64_t uint64;
        float fval;
        double dval;
        struct timeval tv;
        time_t time;
        pmix_status_t status;
        pmix_rank_t rank;
        pmix_nspace_t *nspace;
        pmix_proc_t *proc;
        pmix_byte_object_t bo;
        pmix_persistence_t persist;
        pmix_scope_t scope;
        pmix_data_range_t range;
        pmix_proc_state_t state;
        pmix_proc_info_t *pinfo;
        pmix_data_array_t *darray;
        void *ptr;
        pmix_alloc_directive_t adir;
        pmix_envar_t envar;
        pmix_coord_t *coord;
        pmix_link_state_t linkstate;
        pmix_job_state_t jstate;
        pmix_topology_t *topo;
        pmix_cpuset_t *cpuset;
        pmix_locality_t locality;
        pmix_geometry_t *geometry;
        pmix_device_type_t devtype;
        pmix_device_distance_t *devdist;
        pmix_endpoint_t *endpoint;
        pmix_data_buffer_t *dbuf;
        pmix_proc_stats_t *pstats;
        pmix_disk_stats_t *dkstats;
        pmix_net_stats_t *netstats;
        pmix_node_stats_t *ndstats;
    } data;
} pmix_value_t;

#define PMIX_VALUE_STATIC_INIT  \
{                               \
    .type = PMIX_UNDEF,         \
    .data.ptr = NULL            \
}

/* allocate and initialize a specified number of value structs */
#define PMIX_VALUE_CREATE(m, n)                                             \
    do {                                                                    \
        if (0 == (n)) {                                                     \
            (m) = NULL;                                                     \
        } else {                                                            \
            (m) = (pmix_value_t*)pmix_malloc((n) * sizeof(pmix_value_t));   \
            if (NULL != (m)) {                                              \
                memset((m), 0, (n)*sizeof(pmix_value_t));                   \
            }                                                               \
        }                                                                   \
    } while (0)

/* initialize a single value struct */
#define PMIX_VALUE_CONSTRUCT(m)                 \
    do {                                        \
        memset((m), 0, sizeof(pmix_value_t));   \
        (m)->type = PMIX_UNDEF;                 \
    } while (0)

#define PMIX_VALUE_GET_NUMBER(s, m, n, t)               \
    do {                                                \
        (s) = PMIX_SUCCESS;                             \
        if (PMIX_SIZE == (m)->type) {                   \
            (n) = (t)((m)->data.size);                  \
        } else if (PMIX_INT == (m)->type) {             \
            (n) = (t)((m)->data.integer);               \
        } else if (PMIX_INT8 == (m)->type) {            \
            (n) = (t)((m)->data.int8);                  \
        } else if (PMIX_INT16 == (m)->type) {           \
            (n) = (t)((m)->data.int16);                 \
        } else if (PMIX_INT32 == (m)->type) {           \
            (n) = (t)((m)->data.int32);                 \
        } else if (PMIX_INT64 == (m)->type) {           \
            (n) = (t)((m)->data.int64);                 \
        } else if (PMIX_UINT == (m)->type) {            \
            (n) = (t)((m)->data.uint);                  \
        } else if (PMIX_UINT8 == (m)->type) {           \
            (n) = (t)((m)->data.uint8);                 \
        } else if (PMIX_UINT16 == (m)->type) {          \
            (n) = (t)((m)->data.uint16);                \
        } else if (PMIX_UINT32 == (m)->type) {          \
            (n) = (t)((m)->data.uint32);                \
        } else if (PMIX_UINT64 == (m)->type) {          \
            (n) = (t)((m)->data.uint64);                \
        } else if (PMIX_FLOAT == (m)->type) {           \
            (n) = (t)((m)->data.fval);                  \
        } else if (PMIX_DOUBLE == (m)->type) {          \
            (n) = (t)((m)->data.dval);                  \
        } else if (PMIX_PID == (m)->type) {             \
            (n) = (t)((m)->data.pid);                   \
        } else if (PMIX_PROC_RANK == (m)->type) {       \
            (n) = (t)((m)->data.rank);                  \
        } else {                                        \
            (s) = PMIX_ERR_BAD_PARAM;                   \
        }                                               \
    } while(0)

/****    PMIX INFO STRUCT    ****/
typedef struct pmix_info {
    pmix_key_t key;
    pmix_info_directives_t flags;   // bit-mask of flags
    pmix_value_t value;
} pmix_info_t;

#define PMIX_INFO_STATIC_INIT       \
{                                   \
    .key = {0},                     \
    .flags = 0,                     \
    .value = PMIX_VALUE_STATIC_INIT \
}

/* utility macros for working with pmix_info_t structs */
#define PMIX_INFO_CONSTRUCT(m)                  \
    do {                                        \
        memset((m), 0, sizeof(pmix_info_t));    \
        (m)->value.type = PMIX_UNDEF;           \
    } while (0)

#define PMIX_INFO_CREATE(m, n)                                          \
    do {                                                                \
        pmix_info_t *_i;                                                \
        if (0 == (n)) {                                                 \
            (m) = NULL;                                                 \
        } else {                                                        \
            (m) = (pmix_info_t*)pmix_malloc((n) * sizeof(pmix_info_t)); \
            if (NULL != (m)) {                                          \
                _i = (pmix_info_t*)(m);                                 \
                memset((m), 0, (n) * sizeof(pmix_info_t));              \
                _i[(n)-1].flags = PMIX_INFO_ARRAY_END;                  \
            }                                                           \
        }                                                               \
    } while (0)

/* macros for setting and unsetting the "reqd" flag
 * in a pmix_info_t */
#define PMIX_INFO_REQUIRED(m)       \
    ((m)->flags |= PMIX_INFO_REQD)
#define PMIX_INFO_OPTIONAL(m)       \
    ((m)->flags &= ~PMIX_INFO_REQD)

/* macros for testing the "reqd" flag in a pmix_info_t */
#define PMIX_INFO_IS_REQUIRED(m)    \
    ((m)->flags & PMIX_INFO_REQD)
#define PMIX_INFO_IS_OPTIONAL(m)    \
    !((m)->flags & PMIX_INFO_REQD)

/* macros for setting and testing the "reqd processed" flag */
#define PMIX_INFO_PROCESSED(m)  \
    ((m)->flags |= PMIX_INFO_REQD_PROCESSED)
#define PMIX_INFO_WAS_PROCESSED(m)  \
    ((m)->flags & PMIX_INFO_REQD_PROCESSED)

/* macro for testing end of the array */
#define PMIX_INFO_SET_END(m)    \
    ((m)->flags |= PMIX_INFO_ARRAY_END)
#define PMIX_INFO_IS_END(m)         \
    ((m)->flags & PMIX_INFO_ARRAY_END)

/* macro for testing if qualifier */
#define PMIX_INFO_SET_QUALIFIER(i)   \
    ((i)->flags |= PMIX_INFO_QUALIFIER)
#define PMIX_INFO_IS_QUALIFIER(i)    \
    ((i)->flags & PMIX_INFO_QUALIFIER)

/* macro for setting and testing the "donot release" flag */
#define PMIX_INFO_SET_PERSISTENT(ii) \
    ((ii)->flags |= PMIX_INFO_PERSISTENT)
#define PMIX_INFO_IS_PERSISTENT(ii)  \
    ((ii)->flags & PMIX_INFO_PERSISTENT)

typedef enum {
    PMIX_BOOL_TRUE,
    PMIX_BOOL_FALSE,
    PMIX_NON_BOOL
} pmix_boolean_t;

/**
 * Provide a check to see if a value is "true" or
 * "false", whether given as a string or boolean
 * input.
 */
static inline pmix_boolean_t pmix_check_true(const pmix_value_t *value)
{
    char *ptr;

    if (PMIX_UNDEF == value->type) {
        return PMIX_BOOL_TRUE; // default to true
    }
    if (PMIX_BOOL == value->type) {
        if (value->data.flag) {
            return PMIX_BOOL_TRUE;
        } else {
            return PMIX_BOOL_FALSE;
        }
    }
    if (PMIX_STRING == value->type) {
        if (NULL == value->data.string) {
            return PMIX_BOOL_TRUE;
        }
        ptr = value->data.string;
        /* Trim leading whitespace */
        while (isspace(*ptr)) {
            ++ptr;
        }
        if ('\0' == *ptr) {
            return PMIX_BOOL_TRUE;
        }
        if (isdigit(*ptr)) {
            if (0 == atoi(ptr)) {
                return PMIX_BOOL_FALSE;
            } else {
                return PMIX_BOOL_TRUE;
            }
        } else if (0 == strncasecmp(ptr, "yes", 3) ||
                   0 == strncasecmp(ptr, "true", 4)) {
            return PMIX_BOOL_TRUE;
        } else if (0 == strncasecmp(ptr, "no", 2) ||
                   0 == strncasecmp(ptr, "false", 5)) {
            return PMIX_BOOL_FALSE;
        }
    }

    return PMIX_NON_BOOL;
}

/* provide a macro version of it for those preferring
 * that syntax in their codes where they know the
 * value being checked IS a boolean of some form
 */
#define PMIX_CHECK_TRUE(a) \
    (PMIX_BOOL_TRUE == pmix_check_true(a) ? true : false)

#define PMIX_CHECK_BOOL(a) \
    (PMIX_NON_BOOL == pmix_check_true(a) ? false : true)

/* define a special macro for checking if a boolean
 * info is true - when info structs are provided, a
 * type of PMIX_UNDEF is taken to imply a boolean "true"
 * as the presence of the key defaults to indicating
 * "true". Also supports passing of string representations
 * such as "t" or "f" */
#define PMIX_INFO_TRUE(m)   \
    (PMIX_BOOL_TRUE == pmix_check_true(&(m)->value) ? true : false)


/****    PMIX LOOKUP RETURN STRUCT    ****/
typedef struct pmix_pdata {
    pmix_proc_t proc;
    pmix_key_t key;
    pmix_value_t value;
} pmix_pdata_t;

#define PMIX_LOOKUP_STATIC_INIT     \
{                                   \
    .proc = PMIX_PROC_STATIC_INIT,  \
    .key = {0},                     \
    .value = PMIX_VALUE_STATIC_INIT \
}

/* utility macros for working with pmix_pdata_t structs */
#define PMIX_PDATA_CREATE(m, n)                                             \
    do {                                                                    \
        if (0 == (n)) {                                                     \
            (m) = NULL;                                                     \
        } else {                                                            \
            (m) = (pmix_pdata_t*)pmix_malloc((n) * sizeof(pmix_pdata_t));   \
            if (NULL != (m)) {                                              \
                memset((m), 0, (n) * sizeof(pmix_pdata_t));                 \
            }                                                               \
        }                                                                   \
    } while (0)

#define PMIX_PDATA_CONSTRUCT(m)                 \
    do {                                        \
        memset((m), 0, sizeof(pmix_pdata_t));   \
        (m)->value.type = PMIX_UNDEF;           \
    } while (0)


/****    PMIX APP STRUCT    ****/
typedef struct pmix_app {
    char *cmd;
    char **argv;
    char **env;
    char *cwd;
    int maxprocs;
    pmix_info_t *info;
    size_t ninfo;
} pmix_app_t;

#define PMIX_APP_STATIC_INIT    \
{                               \
    .cmd = NULL,                \
    .argv = NULL,               \
    .env = NULL,                \
    .cwd = NULL,                \
    .maxprocs = 0,              \
    .info = NULL,               \
    .ninfo = 0                  \
}

/* utility macros for working with pmix_app_t structs */
#define PMIX_APP_CREATE(m, n)                                           \
    do {                                                                \
        if (0 == (n)) {                                                 \
            (m) = NULL;                                                 \
        } else {                                                        \
            (m) = (pmix_app_t*)pmix_malloc((n) * sizeof(pmix_app_t));   \
            if (NULL != (m)) {                                          \
                memset((m), 0, (n) * sizeof(pmix_app_t));               \
            }                                                           \
        }                                                               \
    } while (0)

#define PMIX_APP_INFO_CREATE(m, n)                  \
    do {                                            \
        (m)->ninfo = (n);                           \
        PMIX_INFO_CREATE((m)->info, (m)->ninfo);    \
    } while(0)

#define PMIX_APP_RELEASE(m)                     \
    do {                                        \
        PMIX_APP_DESTRUCT((m));                 \
        pmix_free((m));                         \
        (m) = NULL;                             \
    } while (0)

#define PMIX_APP_CONSTRUCT(m)                   \
    do {                                        \
        memset((m), 0, sizeof(pmix_app_t));     \
    } while (0)


/****    PMIX QUERY STRUCT    ****/
typedef struct pmix_query {
    char **keys;
    pmix_info_t *qualifiers;
    size_t nqual;
} pmix_query_t;

#define PMIX_QUERY_STATIC_INIT  \
{                               \
    .keys = NULL,               \
    .qualifiers = NULL,         \
    .nqual = 0                  \
}

/* utility macros for working with pmix_query_t structs */
#define PMIX_QUERY_CREATE(m, n)                                             \
    do {                                                                    \
        if (0 == (n)) {                                                     \
            (m) = NULL;                                                     \
        } else {                                                            \
            (m) = (pmix_query_t*)pmix_malloc((n) * sizeof(pmix_query_t));   \
            if (NULL != (m)) {                                              \
                memset((m), 0, (n) * sizeof(pmix_query_t));                 \
            }                                                               \
        }                                                                   \
    } while (0)

#define PMIX_QUERY_QUALIFIERS_CREATE(m, n)                  \
    do {                                                    \
        (m)->nqual = (n);                                   \
        PMIX_INFO_CREATE((m)->qualifiers, (m)->nqual);      \
    } while(0)

#define PMIX_QUERY_RELEASE(m)       \
    do {                            \
        PMIX_QUERY_DESTRUCT((m));   \
        pmix_free((m));             \
        (m) = NULL;                 \
    } while (0)

#define PMIX_QUERY_CONSTRUCT(m)                 \
    do {                                        \
        memset((m), 0, sizeof(pmix_query_t));   \
    } while (0)

#define PMIX_QUERY_DESTRUCT(m)                                  \
    do {                                                        \
        size_t _qi;                                             \
        if (NULL != (m)->keys) {                                \
            for (_qi=0; NULL != (m)->keys[_qi]; _qi++) {        \
                pmix_free((m)->keys[_qi]);                      \
            }                                                   \
            pmix_free((m)->keys);                               \
            (m)->keys = NULL;                                   \
        }                                                       \
        if (NULL != (m)->qualifiers) {                          \
            PMIX_INFO_FREE((m)->qualifiers, (m)->nqual);        \
            (m)->qualifiers = NULL;                             \
            (m)->nqual = 0;                                     \
        }                                                       \
    } while (0)

#define PMIX_QUERY_FREE(m, n)                       \
    do {                                            \
        size_t _qs;                                 \
        if (NULL != (m)) {                          \
            for (_qs=0; _qs < (n); _qs++) {         \
                PMIX_QUERY_DESTRUCT(&((m)[_qs]));   \
            }                                       \
            pmix_free((m));                         \
            (m) = NULL;                             \
        }                                           \
    } while (0)

/****    ATTRIBUTE REGISTRATION STRUCT   ****/
typedef struct pmix_regattr_t {
    char *name;
    pmix_key_t string;
    pmix_data_type_t type;
    char **description;
} pmix_regattr_t;

#define PMIX_REGATTR_STATIC_INIT    \
{                                   \
    .name = NULL,                   \
    .string = {0},                  \
    .type = PMIX_UNDEF,             \
    .description = NULL             \
}

#define PMIX_REGATTR_CONSTRUCT(a)                       \
    do {                                                \
        if (NULL != (a)) {                              \
            (a)->name = NULL;                           \
            memset((a)->string, 0, PMIX_MAX_KEYLEN+1);  \
            (a)->type = PMIX_UNDEF;                     \
            (a)->description = NULL;                    \
        }                                               \
    } while(0)

#define PMIX_REGATTR_LOAD(a, n, k, t, v)                        \
    do {                                                        \
        pmix_status_t _rgl;                                     \
        if (NULL != (n)) {                                      \
            (a)->name = strdup((n));                            \
        }                                                       \
        if (NULL != (k)) {                                      \
            PMIX_LOAD_KEY((a)->string, (k));                    \
        }                                                       \
        (a)->type = (t);                                        \
        if (NULL != (v)) {                                      \
            PMIX_ARGV_APPEND(_rgl, &(a)->description, (v));     \
        }                                                       \
    } while(0)

#define PMIX_REGATTR_DESTRUCT(a)                    \
    do {                                            \
        if (NULL != (a)) {                          \
            if (NULL != (a)->name) {                \
                pmix_free((a)->name);               \
            }                                       \
            if (NULL != (a)->description) {         \
                PMIX_ARGV_FREE((a)->description);   \
            }                                       \
        }                                           \
    } while(0)

#define PMIX_REGATTR_CREATE(m, n)                                               \
    do {                                                                        \
        if (0 == (n)) {                                                         \
            (m) = NULL;                                                         \
        } else {                                                                \
            (m) = (pmix_regattr_t*)pmix_malloc((n) * sizeof(pmix_regattr_t));   \
            if (NULL != (m)) {                                                  \
                memset((m), 0, (n) * sizeof(pmix_regattr_t));                   \
            }                                                                   \
        }                                                                       \
    } while (0)

#define PMIX_REGATTR_FREE(m, n)                         \
    do {                                                \
        size_t _ra;                                     \
        pmix_regattr_t *_m = (pmix_regattr_t*)(m);      \
        if (NULL != (m)) {                              \
            for (_ra=0; _ra < (n); _ra++) {             \
                PMIX_REGATTR_DESTRUCT(&((_m)[_ra]));    \
            }                                           \
            pmix_free((m));                             \
            (m) = NULL;                                 \
        }                                               \
    } while (0)

#define PMIX_REGATTR_XFER(a, b)                                         \
    do {                                                                \
        size_t _n;                                                      \
        PMIX_REGATTR_CONSTRUCT((a));                                    \
        if (NULL != ((b)->name)) {                                      \
            (a)->name = strdup((b)->name);                              \
        }                                                               \
        PMIX_LOAD_KEY((a)->string, (b)->string);                        \
        (a)->type = (b)->type;                                          \
        if (NULL != (b)->description) {                                 \
            PMIX_ARGV_COPY((a)->description, (b)->description);         \
        }                                                               \
    } while(0)


/****    FABRIC STRUCT    ****/
/* Define a pmix_fabric_t struct for
 * interacting with fabric-related interfaces */
typedef struct pmix_fabric_s {
    /* user-supplied name for this fabric */
    char *name;
    /* a PMIx-supplied index identifying this registration object */
    size_t index;
    /* array containing information (provided by the PMIx library)
     * about the fabric */
    pmix_info_t *info;
    size_t ninfo;
    /* object pointer for use by the PMIx library */
    void *module;
} pmix_fabric_t;

#define PMIX_FABRIC_STATIC_INIT \
{                               \
    .name = NULL,               \
    .index = 0,                 \
    .info = NULL,               \
    .ninfo = 0,                 \
    .module = NULL              \
}

/* convenience macros to support pmix_fabric_t */
#define PMIX_FABRIC_CONSTRUCT(x) \
    memset(x, 0, sizeof(pmix_fabric_t))

typedef enum {
    PMIX_FABRIC_REQUEST_INFO,
    PMIX_FABRIC_UPDATE_INFO
} pmix_fabric_operation_t;

/****    CALLBACK FUNCTIONS FOR NON-BLOCKING OPERATIONS    ****/

typedef void (*pmix_release_cbfunc_t)(void *cbdata);

/* define a callback function that is solely used by servers, and
 * not clients, to return modex data in response to "fence" and "get"
 * operations. The returned blob contains the data collected from each
 * server participating in the operation.
 *
 * As the data is "owned" by the host server, provide a secondary
 * callback function to notify the host server that we are done
 * with the data so it can be released */
typedef void (*pmix_modex_cbfunc_t)(pmix_status_t status,
                                    const char *data, size_t ndata,
                                    void *cbdata,
                                    pmix_release_cbfunc_t release_fn,
                                    void *release_cbdata);

/* define a callback function for calls to PMIx_Spawn_nb - the function
 * will be called upon completion of the spawn command. The status
 * will indicate whether or not the spawn succeeded. The nspace
 * of the spawned processes will be returned, along with any provided
 * callback data. Note that the returned nspace value will be
 * released by the library upon return from the callback function, so
 * the receiver must copy it if it needs to be retained */
typedef void (*pmix_spawn_cbfunc_t)(pmix_status_t status,
                                    pmix_nspace_t nspace, void *cbdata);

/* define a callback for common operations that simply return
 * a status. Examples include the non-blocking versions of
 * Fence, Connect, and Disconnect */
typedef void (*pmix_op_cbfunc_t)(pmix_status_t status, void *cbdata);

/* define a callback function for calls to PMIx_Lookup_nb - the
 * function will be called upon completion of the command with the
 * status indicating the success of failure of the request. Any
 * retrieved data will be returned in an array of pmix_pdata_t structs.
 * The nspace/rank of the process that provided each data element is
 * also returned.
 *
 * Note that these structures will be released upon return from
 * the callback function, so the receiver must copy/protect the
 * data prior to returning if it needs to be retained */

typedef void (*pmix_lookup_cbfunc_t)(pmix_status_t status,
                                     pmix_pdata_t data[], size_t ndata,
                                     void *cbdata);

/* define a callback by which an event handler can notify the PMIx library
 * that it has completed its response to the notification. The handler
 * is _required_ to execute this callback so the library can determine
 * if additional handlers need to be called. The handler shall return
 * PMIX_SUCCESS if no further action is required. The return status
 * of each event handler and any returned pmix_info_t structures
 * will be added to the array of pmix_info_t passed to any subsequent
 * event handlers to help guide their operation.
 *
 * If non-NULL, the provided callback function will be called to allow
 * the event handler to release the provided info array.
 */
typedef void (*pmix_event_notification_cbfunc_fn_t)(pmix_status_t status,
                                                    pmix_info_t *results, size_t nresults,
                                                    pmix_op_cbfunc_t cbfunc, void *thiscbdata,
                                                    void *notification_cbdata);

/* define a callback function for the event handler. Upon receipt of an
 * event notification, PMIx will execute the specified notification
 * callback function, providing:
 *
 * evhdlr_registration_id - the returned registration number of
 *                          the event handler being called
 * status - the event that occurred
 * source - the nspace and rank of the process that generated
 *          the event. If the source is the resource manager,
 *          then the nspace will be empty and the rank will
 *          be PMIX_RANK_UNDEF
 * info - any additional info provided regarding the event.
 * ninfo - the number of info objects in the info array
 * results - any provided results from event handlers called
 *           prior to this one.
 * nresults - number of info objects in the results array
 * cbfunc - the function to be called upon completion of the handler
 * cbdata - pointer to be returned in the completion cbfunc
 *
 * Note that different resource managers may provide differing levels
 * of support for event notification to application processes. Thus, the
 * info array may be NULL or may contain detailed information of the event.
 * It is the responsibility of the application to parse any provided info array
 * for defined key-values if it so desires.
 *
 * Possible uses of the pmix_info_t object include:
 *
 * - for the RM to alert the process as to planned actions, such as
 *   to abort the session, in response to the reported event
 *
 * - provide a timeout for alternative action to occur, such as for
 *   the application to request an alternate response to the event
 *
 * For example, the RM might alert the application to the failure of
 * a node that resulted in termination of several processes, and indicate
 * that the overall session will be aborted unless the application
 * requests an alternative behavior in the next 5 seconds. The application
 * then has time to respond with a checkpoint request, or a request to
 * recover from the failure by obtaining replacement nodes and restarting
 * from some earlier checkpoint.
 *
 * Support for these options is left to the discretion of the host RM. Info
 * keys are included in the common definions above, but also may be augmented
 * on a per-RM basis.
 *
 * On the server side, the notification function is used to inform the host
 * server of a detected event in the PMIx subsystem and/or client
 */
typedef void (*pmix_notification_fn_t)(size_t evhdlr_registration_id,
                                       pmix_status_t status,
                                       const pmix_proc_t *source,
                                       pmix_info_t info[], size_t ninfo,
                                       pmix_info_t *results, size_t nresults,
                                       pmix_event_notification_cbfunc_fn_t cbfunc,
                                       void *cbdata);

/* define a callback function for calls to register handlers, e.g., event
 * notification and IOF requests
 *
 * status - PMIX_SUCCESS or an appropriate error constant
 *
 * refid - reference identifier assigned to the handler by PMIx,
 *         used to deregister the handler
 *
 * cbdata - object provided to the registration call
 */
typedef void (*pmix_hdlr_reg_cbfunc_t)(pmix_status_t status,
                                       size_t refid,
                                       void *cbdata);
/* retain the deprecated form */
typedef void (*pmix_evhdlr_reg_cbfunc_t)(pmix_status_t status,
                                         size_t refid,
                                         void *cbdata);

/* define a callback function for calls to PMIx_Get_nb. The status
 * indicates if the requested data was found or not - a pointer to the
 * pmix_value_t structure containing the found data is returned. The
 * pointer will be NULL if the requested data was not found. */
typedef void (*pmix_value_cbfunc_t)(pmix_status_t status,
                                    pmix_value_t *kv, void *cbdata);

/* define a callback function for calls to PMIx_Query. The status
 * indicates if requested data was found or not - an array of
 * pmix_info_t will contain the key/value pairs. */
typedef void (*pmix_info_cbfunc_t)(pmix_status_t status,
                                   pmix_info_t *info, size_t ninfo,
                                   void *cbdata,
                                   pmix_release_cbfunc_t release_fn,
                                   void *release_cbdata);

/* Define a callback function to return a requested security credential.
 * Returned values include:
 *
 * status - PMIX_SUCCESS if a credential could be assigned as requested, or
 *          else an appropriate error code indicating the problem
 *
 * credential - pointer to an allocated pmix_byte_object_t containing the
 *              credential (as a opaque blob) and its size. Ownership of
 *              the credential is transferred to the receiving function - thus,
 *              responsibility for releasing the memory lies outside the
 *              PMIx library.
 *
 * info - an array of pmix_info_t structures provided by the system to pass
 *        any additional information about the credential - e.g., the identity
 *        of the issuing agent. The info array is owned by the PMIx library
 *        and is not to be released or altered by the receiving party. Note that
 *        this array is not related to the pmix_info_t structures possibly
 *        provided in the call to PMIx_Get_credential.
 *
 *        Information provided by the issuing agent can subsequently be used
 *        by the application for a variety of purposes. Examples include:
 *            - checking identified authorizations to determine what
 *              requests/operations are feasible as a means to steering
 *              workflows
 *            - compare the credential type to that of the local SMS for
 *              compatibility
 *
 * ninfo - number of elements in the info array
 *
 * cbdata - the caller's provided void* object
 *
 * NOTE: the credential is opaque and therefore understandable only by
 *       a service compatible with the issuer.
 */
typedef void (*pmix_credential_cbfunc_t)(pmix_status_t status,
                                         pmix_byte_object_t *credential,
                                         pmix_info_t info[], size_t ninfo,
                                         void *cbdata);


/* Define a validation callback function to indicate if a provided
 * credential is valid, and any corresponding information regarding
 * authorizations and other security matters
 * Returned values include:
 *
 * status - PMIX_SUCCESS if the provided credential is valid. An appropriate
 *          error code indicating the issue if the credential is rejected.
 *
 * info - an array of pmix_info_t structures provided by the system to pass
 *        any additional information about the authentication - e.g., the
 *        effective userid and group id of the certificate holder, and any
 *        related authorizations. The info array is owned by the PMIx library
 *        and is not to be released or altered by the receiving party. Note that
 *        this array is not related to the pmix_info_t structures possibly
 *        provided in the call to PMIx_Validate_credential.
 *
 *        The precise contents of the array will depend on the host SMS and
 *        its associated security system. At the minimum, it is expected (but
 *        not required) that the array will contain entries for the PMIX_USERID
 *        and PMIX_GROUPID of the client described in the credential.
 *
 * ninfo - number of elements in the info array
 *
 * cbdata - the caller's provided void* object
 */
typedef void (*pmix_validation_cbfunc_t)(pmix_status_t status,
                                         pmix_info_t info[], size_t ninfo,
                                         void *cbdata);

/* Define a callback function to return device distance arrays
 */
typedef void (*pmix_device_dist_cbfunc_t)(pmix_status_t status,
                                          pmix_device_distance_t *dist,
                                          size_t ndist,
                                          void *cbdata,
                                          pmix_release_cbfunc_t release_fn,
                                          void *release_cbdata);



#define PMIX_DATA_ARRAY_INIT(m, t)      \
    do {                                \
        (m)->array = NULL;              \
        (m)->type = (t);                \
        (m)->size = 0;                  \
    } while(0)

#define PMIX_DATA_ARRAY_CONSTRUCT(m, n, t)                                  \
    do {                                                                    \
        (m)->type = (t);                                                    \
        (m)->size = (n);                                                    \
        if (0 < (n)) {                                                      \
            if (PMIX_INFO == (t)) {                                         \
                PMIX_INFO_CREATE((m)->array, (n));                          \
                                                                            \
            } else if (PMIX_PROC == (t)) {                                  \
                PMIX_PROC_CREATE((m)->array, (n));                          \
                                                                            \
            } else if (PMIX_PROC_INFO == (t)) {                             \
                PMIX_PROC_INFO_CREATE((m)->array, (n));                     \
                                                                            \
            } else if (PMIX_ENVAR == (t)) {                                 \
                PMIX_ENVAR_CREATE((m)->array, (n));                         \
                                                                            \
            } else if (PMIX_VALUE == (t)) {                                 \
                PMIX_VALUE_CREATE((m)->array, (n));                         \
                                                                            \
            } else if (PMIX_PDATA == (t)) {                                 \
                PMIX_PDATA_CREATE((m)->array, (n));                         \
                                                                            \
            } else if (PMIX_QUERY == (t)) {                                 \
                PMIX_QUERY_CREATE((m)->array, (n));                         \
                                                                            \
            } else if (PMIX_APP == (t)) {                                   \
                PMIX_APP_CREATE((m)->array, (n));                           \
                                                                            \
            } else if (PMIX_BYTE_OBJECT == (t) ||                           \
                       PMIX_COMPRESSED_STRING == (t)) {                     \
                PMIX_BYTE_OBJECT_CREATE((m)->array, (n));                   \
                                                                            \
            } else if (PMIX_ALLOC_DIRECTIVE == (t) ||                       \
                       PMIX_PROC_STATE == (t) ||                            \
                       PMIX_PERSIST == (t) ||                               \
                       PMIX_SCOPE == (t) ||                                 \
                       PMIX_DATA_RANGE == (t) ||                            \
                       PMIX_BYTE == (t) ||                                  \
                       PMIX_INT8 == (t) ||                                  \
                       PMIX_UINT8 == (t) ||                                 \
                       PMIX_POINTER == (t)) {                               \
                (m)->array = pmix_calloc((n), sizeof(int8_t));              \
                                                                            \
            } else if (PMIX_STRING == (t)) {                                \
                (m)->array = pmix_calloc((n), sizeof(char*));               \
                                                                            \
            } else if (PMIX_SIZE == (t)) {                                  \
                (m)->array = pmix_calloc((n), sizeof(size_t));              \
                                                                            \
            } else if (PMIX_PID == (t)) {                                   \
                (m)->array = pmix_calloc((n), sizeof(pid_t));               \
                                                                            \
            } else if (PMIX_INT == (t) ||                                   \
                       PMIX_UINT == (t) ||                                  \
                       PMIX_STATUS == (t)) {                                \
                (m)->array = pmix_calloc((n), sizeof(int));                 \
                                                                            \
            } else if (PMIX_IOF_CHANNEL == (t) ||                           \
                       PMIX_DATA_TYPE == (t) ||                             \
                       PMIX_INT16 == (t) ||                                 \
                       PMIX_UINT16 == (t)) {                                \
                (m)->array = pmix_calloc((n), sizeof(int16_t));             \
                                                                            \
            } else if (PMIX_PROC_RANK == (t) ||                             \
                       PMIX_INFO_DIRECTIVES == (t) ||                       \
                       PMIX_INT32 == (t) ||                                 \
                       PMIX_UINT32 == (t)) {                                \
                (m)->array = pmix_calloc((n), sizeof(int32_t));             \
                                                                            \
            } else if (PMIX_INT64 == (t) ||                                 \
                       PMIX_UINT64 == (t)) {                                \
                (m)->array = pmix_calloc((n), sizeof(int64_t));             \
                                                                            \
            } else if (PMIX_FLOAT == (t)) {                                 \
                (m)->array = pmix_calloc((n), sizeof(float));               \
                                                                            \
            } else if (PMIX_DOUBLE == (t)) {                                \
                (m)->array = pmix_calloc((n), sizeof(double));              \
                                                                            \
            } else if (PMIX_TIMEVAL == (t)) {                               \
                (m)->array = pmix_calloc((n), sizeof(struct timeval));      \
                                                                            \
            } else if (PMIX_TIME == (t)) {                                  \
                (m)->array = pmix_calloc((n), sizeof(time_t));              \
                                                                            \
            } else if (PMIX_REGATTR == (t)) {                               \
                PMIX_REGATTR_CREATE((m)->array, (n));                       \
                                                                            \
            } else if (PMIX_BOOL == (t)) {                                  \
                (m)->array = pmix_calloc((n), sizeof(bool));                \
                                                                            \
            } else if (PMIX_COORD == (t)) {                                 \
                (m)->array = pmix_calloc((n), sizeof(pmix_coord_t));        \
                                                                            \
            } else if (PMIX_LINK_STATE == (t)) {                            \
                (m)->array = pmix_calloc((n), sizeof(pmix_link_state_t));   \
                                                                            \
            } else if (PMIX_ENDPOINT == (t)) {                              \
                PMIX_ENDPOINT_CREATE((m)->array, n);                        \
                                                                            \
            } else if (PMIX_PROC_NSPACE == (t)) {                           \
                (m)->array = pmix_calloc((n), sizeof(pmix_nspace_t));       \
                                                                            \
            } else if (PMIX_PROC_STATS == (t)) {                            \
                PMIX_PROC_STATS_CREATE((m)->array, n);                      \
                                                                            \
            } else if (PMIX_DISK_STATS == (t)) {                            \
                PMIX_DISK_STATS_CREATE((m)->array, n);                      \
                                                                            \
            } else if (PMIX_NET_STATS == (t)) {                             \
                PMIX_NET_STATS_CREATE((m)->array, n);                       \
                                                                            \
            } else if (PMIX_NODE_STATS == (t)) {                            \
                PMIX_NODE_STATS_CREATE((m)->array, n);                      \
                                                                            \
            } else if (PMIX_DEVICE_DIST == (t)) {                           \
                PMIX_DEVICE_DIST_CREATE((m)->array, n);                     \
                                                                            \
            } else if (PMIX_GEOMETRY == (t)) {                              \
                PMIX_GEOMETRY_CREATE((m)->array, n);                        \
                                                                            \
            } else if (PMIX_REGATTR == (t)) {                               \
                PMIX_REGATTR_CREATE((m)->array, n);                         \
                                                                            \
            } else if (PMIX_PROC_CPUSET == (t)) {                           \
                PMIX_CPUSET_CREATE((m)->array, n);                          \
            } else {                                                        \
                (m)->array = NULL;                                          \
                (m)->size = 0;                                              \
            }                                                               \
        } else {                                                            \
            (m)->array = NULL;                                              \
        }                                                                   \
    } while(0)
#define PMIX_DATA_ARRAY_CREATE(m, n, t)                                     \
    do {                                                                    \
        (m) = (pmix_data_array_t*)pmix_malloc(sizeof(pmix_data_array_t));   \
        if (NULL != (m)) {                                                  \
            memset((m), 0, sizeof(pmix_data_array_t));                      \
            PMIX_DATA_ARRAY_CONSTRUCT((m), (n), (t));                       \
        }                                                                   \
    } while(0)

#include <pmix_deprecated.h>

/********    STANDARD MACROS FOR DARRAY AND VALUE SUPPORT     ********/

/* release the memory in the value struct data field */
#define PMIX_VALUE_DESTRUCT(m) PMIx_Value_destruct(m)

/* release a single pmix_value_t struct, including its data */
#define PMIX_VALUE_RELEASE(m)       \
    do {                            \
        PMIX_VALUE_DESTRUCT((m));   \
        pmix_free((m));             \
        (m) = NULL;                 \
    } while (0)

#define PMIX_VALUE_FREE(m, n)                           \
    do {                                                \
        size_t _vv;                                     \
        if (NULL != (m)) {                              \
            for (_vv=0; _vv < (n); _vv++) {             \
                PMIX_VALUE_DESTRUCT(&((m)[_vv]));       \
            }                                           \
            pmix_free((m));                             \
            (m) = NULL;                                 \
        }                                               \
    } while (0)

#define PMIX_INFO_DESTRUCT(m)                   \
    do {                                        \
        if (!PMIX_INFO_IS_PERSISTENT((m))) {    \
            PMIX_VALUE_DESTRUCT(&(m)->value);   \
        }                                       \
    } while (0)

#define PMIX_INFO_FREE(m, n)                        \
    do {                                            \
        size_t _is;                                 \
        if (NULL != (m)) {                          \
            for (_is=0; _is < (n); _is++) {         \
                PMIX_INFO_DESTRUCT(&((m)[_is]));    \
            }                                       \
            pmix_free((m));                         \
            (m) = NULL;                             \
        }                                           \
    } while (0)

#define PMIX_APP_DESTRUCT(m)                                    \
    do {                                                        \
        if (NULL != (m)->cmd) {                                 \
            pmix_free((m)->cmd);                                \
            (m)->cmd = NULL;                                    \
        }                                                       \
        if (NULL != (m)->argv) {                                \
            pmix_argv_free((m)->argv);                          \
            (m)->argv = NULL;                                   \
        }                                                       \
        if (NULL != (m)->env) {                                 \
            pmix_argv_free((m)->env);                           \
            (m)->env = NULL;                                    \
        }                                                       \
        if (NULL != (m)->cwd) {                                 \
            pmix_free((m)->cwd);                                \
            (m)->cwd = NULL;                                    \
        }                                                       \
        if (NULL != (m)->info) {                                \
            PMIX_INFO_FREE((m)->info, (m)->ninfo);              \
            (m)->info = NULL;                                   \
            (m)->ninfo = 0;                                     \
        }                                                       \
    } while (0)

static inline void pmix_app_free(pmix_app_t *ap, size_t n)
{
    size_t k;

    if (NULL != ap) {
        for (k=0; k < n; k++) {
            PMIX_APP_DESTRUCT(&ap[k]);
        }
    }
}

#define PMIX_APP_FREE(m, n)     \
    do {                        \
        pmix_app_free(m, n);    \
        pmix_free(m);           \
        (m) = NULL;             \
    } while (0)


#define PMIX_DATA_ARRAY_DESTRUCT(m) PMIx_Data_array_destruct(m)

#define PMIX_DATA_ARRAY_FREE(m)             \
    do {                                    \
        if (NULL != (m)) {                  \
            PMIX_DATA_ARRAY_DESTRUCT(m);    \
            pmix_free((m));                 \
            (m) = NULL;                     \
        }                                   \
    } while(0)

#define PMIX_PDATA_RELEASE(m)                   \
    do {                                        \
        PMIX_VALUE_DESTRUCT(&(m)->value);       \
        pmix_free((m));                         \
        (m) = NULL;                             \
    } while (0)

#define PMIX_PDATA_DESTRUCT(m)                  \
    do {                                        \
        PMIX_VALUE_DESTRUCT(&(m)->value);       \
    } while (0)

static inline void pmix_pdata_free(pmix_pdata_t *pd, size_t n)
{
    size_t k;

    if (NULL != pd) {
        for (k=0; k < n; k++) {
            PMIX_PDATA_DESTRUCT(&pd[k]);
        }
    }
}

#define PMIX_PDATA_FREE(m, n)   \
do {                            \
    pmix_pdata_free(m, n);      \
    pmix_free(m);               \
    (m) = NULL;                 \
} while(0)

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif
