/*
 * Copyright (c) 2014-2017 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * DOC: i_qdf_trace.h
 *
 * Linux-specific definitions for QDF trace
 *
 */

#if !defined(__I_QDF_TRACE_H)
#define __I_QDF_TRACE_H

/* older kernels have a bug in kallsyms, so ensure module.h is included */
#include <linux/module.h>
#include <linux/kallsyms.h>

#if !defined(__printf)
#define __printf(a, b)
#endif

#ifdef CONFIG_MCL
/* QDF_TRACE is the macro invoked to add trace messages to code.  See the
 * documenation for qdf_trace_msg() for the parameters etc. for this function.
 *
 * NOTE:  Code QDF_TRACE() macros into the source code.  Do not code directly
 * to the qdf_trace_msg() function.
 *
 * NOTE 2:  qdf tracing is totally turned off if WLAN_DEBUG is *not* defined.
 * This allows us to build 'performance' builds where we can measure performance
 * without being bogged down by all the tracing in the code
 */
#if defined(WLAN_DEBUG) || defined(DEBUG)
#define QDF_TRACE qdf_trace_msg
#define QDF_VTRACE qdf_vtrace_msg
#define QDF_TRACE_HEX_DUMP qdf_trace_hex_dump
#define QDF_TRACE_RATE_LIMITED(rate, module, level, format, ...)\
	do {\
		static int rate_limit;\
		rate_limit++;\
		if (rate)\
			if (0 == (rate_limit % rate))\
				qdf_trace_msg(module, level, format,\
						##__VA_ARGS__);\
	} while (0)
#else
#define QDF_TRACE(arg ...)
#define QDF_VTRACE(arg ...)
#define QDF_TRACE_HEX_DUMP(arg ...)
#define QDF_TRACE_RATE_LIMITED(arg ...)
#endif
#else

#define qdf_trace(log_level, args...) \
		do {	\
			extern int qdf_dbg_mask; \
			if (qdf_dbg_mask >= log_level) { \
				printk("qdf: "args); \
				printk("\n"); \
			} \
		} while (0)
#define QDF_TRACE(x, y, args...) printk(args)

#endif /* CONFIG_MCL */

#define QDF_ENABLE_TRACING
#define qdf_scnprintf scnprintf

#ifdef QDF_ENABLE_TRACING

#define QDF_ASSERT(_condition) \
	do { \
		if (!(_condition)) { \
			pr_err("QDF ASSERT in %s Line %d\n", \
			       __func__, __LINE__); \
			WARN_ON(1); \
		} \
	} while (0)

#else

/* This code will be used for compilation if tracing is to be compiled out */
/* of the code so these functions/macros are 'do nothing' */
static inline void qdf_trace_msg(QDF_MODULE_ID module, ...)
{
}

#define QDF_ASSERT(_condition)

#endif

#ifdef PANIC_ON_BUG

#define QDF_BUG(_condition) \
	do { \
		if (!(_condition)) { \
			pr_err("QDF BUG in %s Line %d\n", \
			       __func__, __LINE__); \
			BUG_ON(1); \
		} \
	} while (0)

#else

#define QDF_BUG(_condition) \
	do { \
		if (!(_condition)) { \
			pr_err("QDF BUG in %s Line %d\n", \
			       __func__, __LINE__); \
			WARN_ON(1); \
		} \
	} while (0)

#endif

#ifdef KSYM_SYMBOL_LEN
#define __QDF_SYMBOL_LEN KSYM_SYMBOL_LEN
#else
#define __QDF_SYMBOL_LEN 1
#endif

#endif /* __I_QDF_TRACE_H */
