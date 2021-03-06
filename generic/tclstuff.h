// Written by Cyan Ogilvie, and placed in the public domain

#ifndef _TCLSTUFF_H
#define _TCLSTUFF_H

#include "tcl.h"

#define NEW_CMD( tcl_cmd, c_cmd ) \
	Tcl_CreateObjCommand( interp, tcl_cmd, \
			(Tcl_ObjCmdProc *) c_cmd, \
			(ClientData *) NULL, NULL );

#define THROW_ERROR( ... )								\
	{													\
		Tcl_AppendResult(interp, ##__VA_ARGS__, NULL);	\
		return TCL_ERROR;								\
	}

#define THROW_PRINTF( fmtstr, ... )											\
	{																		\
		Tcl_SetObjResult(interp, Tcl_ObjPrintf((fmtstr), ##__VA_ARGS__));	\
		return TCL_ERROR;													\
	}

#define THROW_ERROR_LABEL( label, var, ... )				\
	{														\
		Tcl_AppendResult(interp, ##__VA_ARGS__, NULL);		\
		var = TCL_ERROR;									\
		goto label;											\
	}

// convenience macro to check the number of arguments passed to a function
// implementing a tcl command against the number expected, and to throw
// a tcl error if they don't match.  Note that the value of expected does
// not include the objv[0] object (the function itself)
#define CHECK_ARGS(expected, msg)										\
	if (objc != expected + 1) {											\
		Tcl_ResetResult( interp );										\
		Tcl_AppendResult( interp, "Wrong # of arguments.  Must be \"",	\
						  msg, "\"", NULL );							\
		return TCL_ERROR;												\
	}


// A rather frivolous macro that just enhances readability for a common case
#ifdef DEBUG
#warning Using DEBUG version of TEST_OK
#define TEST_OK( cmd )		\
	if (cmd != TCL_OK) { \
		fprintf( stderr, "TEST_OK() returned TCL_ERROR\n" ); \
		return TCL_ERROR; \
	}
#define TEST_OK_LABEL( label, var, cmd )		\
	if (cmd != TCL_OK) { \
		var = TCL_ERROR; \
		goto label; \
	}
#else
#define TEST_OK( cmd )		\
	if (cmd != TCL_OK) return TCL_ERROR;
#endif

#define TEST_OK_LABEL( label, var, cmd )		\
	if (cmd != TCL_OK) { \
		var = TCL_ERROR; \
		goto label; \
	}

#define TEST_OK_BREAK(var, cmd) if (TCL_OK != (var=(cmd))) break;

static inline void release_tclobj(Tcl_Obj** obj)
{
	if (*obj) {
		Tcl_DecrRefCount(*obj);
		*obj = NULL;
	}
}
#define RELEASE_MACRO(obj)		if (obj) {Tcl_DecrRefCount(obj); obj=NULL;}
#define REPLACE_MACRO(target, replacement)	\
{ \
	release_tclobj(&target); \
	if (replacement) Tcl_IncrRefCount(target = replacement); \
}
static inline void replace_tclobj(Tcl_Obj** target, Tcl_Obj* replacement)
{
	if (*target) {
		Tcl_DecrRefCount(*target);
		*target = NULL;
	}
	*target = replacement;
	if (*target) Tcl_IncrRefCount(*target);
}

#include <signal.h>
#define DEBUGGER raise(SIGTRAP)

#endif
