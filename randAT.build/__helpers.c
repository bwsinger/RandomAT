// This file contains helper functions that are automatically created from
// templates.

#include "nuitka/prelude.h"

extern PyObject *callPythonFunction( PyObject *func, PyObject **args, int count );


PyObject *CALL_FUNCTION_WITH_ARGS1( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 1; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 1 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 1; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 1 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 1 * sizeof(PyObject *) );
            memcpy( python_pars + 1, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 1 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 1 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 1; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 1 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 1 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 1, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 1 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 1 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (1 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 1 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (1 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 1 );

            PyObject *result;

            assert( flags && METH_VARARGS );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags && METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 1, NULL );;
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            1
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 1 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS2( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 2; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 2 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 2; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 2 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 2 * sizeof(PyObject *) );
            memcpy( python_pars + 2, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 2 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 2 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 2; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 2 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 2 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 2, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 2 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 2 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (2 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 2 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (2 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 2 );

            PyObject *result;

            assert( flags && METH_VARARGS );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags && METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 2, NULL );;
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            2
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 2 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS3( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 3; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 3 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 3; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 3 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 3 * sizeof(PyObject *) );
            memcpy( python_pars + 3, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 3 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 3 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 3; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 3 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 3 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 3, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 3 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 3 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (3 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 3 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (3 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 3 );

            PyObject *result;

            assert( flags && METH_VARARGS );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags && METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 3, NULL );;
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            3
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 3 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS4( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 4; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 4 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 4; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 4 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 4 * sizeof(PyObject *) );
            memcpy( python_pars + 4, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 4 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 4 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 4; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 4 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 4 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 4, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 4 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 4 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (4 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 4 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (4 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 4 );

            PyObject *result;

            assert( flags && METH_VARARGS );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags && METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 4, NULL );;
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            4
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 4 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}
/* Code to register embedded modules for meta path based loading if any. */

#include "nuitka/unfreezing.h"

/* Table for lookup to find compiled or bytecode modules included in this
 * binary or module, or put along this binary as extension modules. We do
 * our own loading for each of these.
 */
MOD_INIT_DECL( __main__ );
static struct Nuitka_MetaPathBasedLoaderEntry meta_path_loader_entries[] =
{
    { (char *)"_File", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"__main__", MOD_INIT_NAME( __main__ ), 0, 0, NUITKA_COMPILED_MODULE },
    { (char *)"_bisect", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_codecs_cn", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_codecs_hk", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_codecs_iso2022", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_codecs_jp", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_codecs_kr", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_codecs_tw", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_collections", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_csv", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_ctypes", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_curses", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_curses_panel", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_elementtree", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_functools", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_hashlib", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_heapq", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_hotshot", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_io", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_json", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_locale", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_lsprof", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_multibytecodec", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_multiprocessing", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_random", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_scproxy", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_socket", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_sqlite3", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_ssl", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_struct", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_tkinter", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"array", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"binascii", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"bz2", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"cPickle", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"cStringIO", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"datetime", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"dbm", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"fcntl", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"grp", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"itertools", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"math", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"mmap", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"operator", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"parser", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"pyexpat", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"readline", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"resource", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"select", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"strop", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"termios", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"time", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"unicodedata", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"zlib", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"BaseHTTPServer", NULL, 2301, 22689, NUITKA_BYTECODE_FLAG },
    { (char *)"Bastion", NULL, 24990, 7402, NUITKA_BYTECODE_FLAG },
    { (char *)"CGIHTTPServer", NULL, 32392, 11752, NUITKA_BYTECODE_FLAG },
    { (char *)"Canvas", NULL, 44144, 19556, NUITKA_BYTECODE_FLAG },
    { (char *)"Carbon", NULL, 63700, 465, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"Carbon.File", NULL, 64165, 209, NUITKA_BYTECODE_FLAG },
    { (char *)"ConfigParser", NULL, 64374, 28155, NUITKA_BYTECODE_FLAG },
    { (char *)"Cookie", NULL, 92529, 24549, NUITKA_BYTECODE_FLAG },
    { (char *)"Dialog", NULL, 117078, 2162, NUITKA_BYTECODE_FLAG },
    { (char *)"DocXMLRPCServer", NULL, 119240, 10727, NUITKA_BYTECODE_FLAG },
    { (char *)"FileDialog", NULL, 129967, 10816, NUITKA_BYTECODE_FLAG },
    { (char *)"FixTk", NULL, 140783, 2140, NUITKA_BYTECODE_FLAG },
    { (char *)"HTMLParser", NULL, 142923, 15326, NUITKA_BYTECODE_FLAG },
    { (char *)"MimeWriter", NULL, 158249, 7806, NUITKA_BYTECODE_FLAG },
    { (char *)"Queue", NULL, 166055, 10816, NUITKA_BYTECODE_FLAG },
    { (char *)"ScrolledText", NULL, 176871, 2906, NUITKA_BYTECODE_FLAG },
    { (char *)"SimpleDialog", NULL, 179777, 4881, NUITKA_BYTECODE_FLAG },
    { (char *)"SimpleHTTPServer", NULL, 184658, 8608, NUITKA_BYTECODE_FLAG },
    { (char *)"SimpleXMLRPCServer", NULL, 193266, 24654, NUITKA_BYTECODE_FLAG },
    { (char *)"SocketServer", NULL, 217920, 26900, NUITKA_BYTECODE_FLAG },
    { (char *)"StringIO", NULL, 244820, 12422, NUITKA_BYTECODE_FLAG },
    { (char *)"Tix", NULL, 257242, 112704, NUITKA_BYTECODE_FLAG },
    { (char *)"Tkconstants", NULL, 369946, 2288, NUITKA_BYTECODE_FLAG },
    { (char *)"Tkdnd", NULL, 372234, 14117, NUITKA_BYTECODE_FLAG },
    { (char *)"Tkinter", NULL, 386351, 227789, NUITKA_BYTECODE_FLAG },
    { (char *)"UserDict", NULL, 614140, 11062, NUITKA_BYTECODE_FLAG },
    { (char *)"UserList", NULL, 625202, 8269, NUITKA_BYTECODE_FLAG },
    { (char *)"UserString", NULL, 633471, 18256, NUITKA_BYTECODE_FLAG },
    { (char *)"_LWPCookieJar", NULL, 651727, 5824, NUITKA_BYTECODE_FLAG },
    { (char *)"_MozillaCookieJar", NULL, 657551, 4653, NUITKA_BYTECODE_FLAG },
    { (char *)"__future__", NULL, 662204, 4584, NUITKA_BYTECODE_FLAG },
    { (char *)"_abcoll", NULL, 666788, 30742, NUITKA_BYTECODE_FLAG },
    { (char *)"_osx_support", NULL, 697530, 12740, NUITKA_BYTECODE_FLAG },
    { (char *)"_pyio", NULL, 710270, 73170, NUITKA_BYTECODE_FLAG },
    { (char *)"_strptime", NULL, 783440, 15867, NUITKA_BYTECODE_FLAG },
    { (char *)"_sysconfigdata", NULL, 799307, 20588, NUITKA_BYTECODE_FLAG },
    { (char *)"_threading_local", NULL, 819895, 7046, NUITKA_BYTECODE_FLAG },
    { (char *)"_weakrefset", NULL, 826941, 12070, NUITKA_BYTECODE_FLAG },
    { (char *)"abc", NULL, 839011, 6685, NUITKA_BYTECODE_FLAG },
    { (char *)"aifc", NULL, 845696, 34123, NUITKA_BYTECODE_FLAG },
    { (char *)"anydbm", NULL, 879819, 2942, NUITKA_BYTECODE_FLAG },
    { (char *)"ast", NULL, 882761, 14146, NUITKA_BYTECODE_FLAG },
    { (char *)"asynchat", NULL, 896907, 10311, NUITKA_BYTECODE_FLAG },
    { (char *)"asyncore", NULL, 907218, 21782, NUITKA_BYTECODE_FLAG },
    { (char *)"atexit", NULL, 929000, 2495, NUITKA_BYTECODE_FLAG },
    { (char *)"audiodev", NULL, 931495, 9811, NUITKA_BYTECODE_FLAG },
    { (char *)"bdb", NULL, 941306, 22143, NUITKA_BYTECODE_FLAG },
    { (char *)"binhex", NULL, 963449, 18001, NUITKA_BYTECODE_FLAG },
    { (char *)"bisect", NULL, 981450, 3313, NUITKA_BYTECODE_FLAG },
    { (char *)"bsddb", NULL, 984763, 14292, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"cProfile", NULL, 999055, 7000, NUITKA_BYTECODE_FLAG },
    { (char *)"calendar", NULL, 1006055, 31731, NUITKA_BYTECODE_FLAG },
    { (char *)"cgi", NULL, 1037786, 36005, NUITKA_BYTECODE_FLAG },
    { (char *)"cgitb", NULL, 1073791, 13076, NUITKA_BYTECODE_FLAG },
    { (char *)"chunk", NULL, 1086867, 6143, NUITKA_BYTECODE_FLAG },
    { (char *)"cmd", NULL, 1093010, 15081, NUITKA_BYTECODE_FLAG },
    { (char *)"code", NULL, 1108091, 11126, NUITKA_BYTECODE_FLAG },
    { (char *)"codeop", NULL, 1119217, 7089, NUITKA_BYTECODE_FLAG },
    { (char *)"collections", NULL, 1126306, 28213, NUITKA_BYTECODE_FLAG },
    { (char *)"colorsys", NULL, 1154519, 4383, NUITKA_BYTECODE_FLAG },
    { (char *)"commands", NULL, 1158902, 2761, NUITKA_BYTECODE_FLAG },
    { (char *)"compileall", NULL, 1161663, 7309, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler", NULL, 1168972, 1339, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"compiler.ast", NULL, 1170311, 91567, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.consts", NULL, 1261878, 779, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.future", NULL, 1262657, 3538, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.misc", NULL, 1266195, 4675, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.pyassem", NULL, 1270870, 29991, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.pycodegen", NULL, 1300861, 65157, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.symbols", NULL, 1366018, 20469, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.syntax", NULL, 1386487, 2174, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.transformer", NULL, 1388661, 53872, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.visitor", NULL, 1442533, 4679, NUITKA_BYTECODE_FLAG },
    { (char *)"contextlib", NULL, 1447212, 5046, NUITKA_BYTECODE_FLAG },
    { (char *)"cookielib", NULL, 1452258, 59611, NUITKA_BYTECODE_FLAG },
    { (char *)"copy", NULL, 1511869, 13486, NUITKA_BYTECODE_FLAG },
    { (char *)"csv", NULL, 1525355, 14962, NUITKA_BYTECODE_FLAG },
    { (char *)"ctypes", NULL, 1540317, 23260, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"ctypes._endian", NULL, 1563577, 2599, NUITKA_BYTECODE_FLAG },
    { (char *)"ctypes.macholib", NULL, 1566176, 358, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"ctypes.macholib.dyld", NULL, 1566534, 6579, NUITKA_BYTECODE_FLAG },
    { (char *)"ctypes.macholib.dylib", NULL, 1573113, 2507, NUITKA_BYTECODE_FLAG },
    { (char *)"ctypes.macholib.framework", NULL, 1575620, 2817, NUITKA_BYTECODE_FLAG },
    { (char *)"ctypes.util", NULL, 1578437, 8636, NUITKA_BYTECODE_FLAG },
    { (char *)"curses", NULL, 1587073, 1695, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"curses.ascii", NULL, 1588768, 6132, NUITKA_BYTECODE_FLAG },
    { (char *)"curses.has_key", NULL, 1594900, 6029, NUITKA_BYTECODE_FLAG },
    { (char *)"curses.panel", NULL, 1600929, 319, NUITKA_BYTECODE_FLAG },
    { (char *)"curses.textpad", NULL, 1601248, 7362, NUITKA_BYTECODE_FLAG },
    { (char *)"curses.wrapper", NULL, 1608610, 1310, NUITKA_BYTECODE_FLAG },
    { (char *)"dbhash", NULL, 1609920, 810, NUITKA_BYTECODE_FLAG },
    { (char *)"decimal", NULL, 1610730, 184110, NUITKA_BYTECODE_FLAG },
    { (char *)"difflib", NULL, 1794840, 64641, NUITKA_BYTECODE_FLAG },
    { (char *)"dircache", NULL, 1859481, 1768, NUITKA_BYTECODE_FLAG },
    { (char *)"dis", NULL, 1861249, 6620, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils", NULL, 1867869, 428, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"distutils.archive_util", NULL, 1868297, 7848, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.bcppcompiler", NULL, 1876145, 8324, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.ccompiler", NULL, 1884469, 39572, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.cmd", NULL, 1924041, 18646, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command", NULL, 1942687, 707, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"distutils.command.bdist", NULL, 1943394, 5511, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.bdist_dumb", NULL, 1948905, 5285, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.bdist_msi", NULL, 1954190, 24855, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.bdist_rpm", NULL, 1979045, 18192, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.bdist_wininst", NULL, 1997237, 11285, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.build", NULL, 2008522, 5639, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.build_clib", NULL, 2014161, 6925, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.build_ext", NULL, 2021086, 20307, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.build_py", NULL, 2041393, 12575, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.build_scripts", NULL, 2053968, 4876, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.check", NULL, 2058844, 6860, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.clean", NULL, 2065704, 3418, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.config", NULL, 2069122, 13682, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.install", NULL, 2082804, 18489, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.install_data", NULL, 2101293, 3506, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.install_egg_info", NULL, 2104799, 4208, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.install_headers", NULL, 2109007, 2638, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.install_lib", NULL, 2111645, 7376, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.install_scripts", NULL, 2119021, 3340, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.register", NULL, 2122361, 10864, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.sdist", NULL, 2133225, 17593, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.upload", NULL, 2150818, 6620, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.config", NULL, 2157438, 3912, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.core", NULL, 2161350, 7842, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.cygwinccompiler", NULL, 2169192, 10417, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.debug", NULL, 2179609, 296, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.dep_util", NULL, 2179905, 3372, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.dir_util", NULL, 2183277, 7128, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.dist", NULL, 2190405, 42913, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.emxccompiler", NULL, 2233318, 7909, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.errors", NULL, 2241227, 7277, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.extension", NULL, 2248504, 7604, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.fancy_getopt", NULL, 2256108, 13000, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.file_util", NULL, 2269108, 6992, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.filelist", NULL, 2276100, 11598, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.log", NULL, 2287698, 3378, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.msvc9compiler", NULL, 2291076, 23068, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.msvccompiler", NULL, 2314144, 18817, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.spawn", NULL, 2332961, 6741, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.sysconfig", NULL, 2339702, 14829, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests", NULL, 2354531, 1560, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"distutils.tests.setuptools_build_ext", NULL, 2356091, 10949, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.setuptools_extension", NULL, 2367040, 2396, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.support", NULL, 2369436, 10446, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_archive_util", NULL, 2379882, 11389, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_bdist", NULL, 2391271, 2269, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_bdist_dumb", NULL, 2393540, 4623, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_bdist_msi", NULL, 2398163, 1549, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_bdist_rpm", NULL, 2399712, 5226, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_bdist_wininst", NULL, 2404938, 1557, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_build", NULL, 2406495, 2234, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_build_clib", NULL, 2408729, 5370, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_build_ext", NULL, 2414099, 16876, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_build_py", NULL, 2430975, 5049, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_build_scripts", NULL, 2436024, 4295, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_ccompiler", NULL, 2440319, 4904, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_check", NULL, 2445223, 4936, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_clean", NULL, 2450159, 2079, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_cmd", NULL, 2452238, 5963, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_config", NULL, 2458201, 4789, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_config_cmd", NULL, 2462990, 4396, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_core", NULL, 2467386, 4299, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_dep_util", NULL, 2471685, 3179, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_dir_util", NULL, 2474864, 5856, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_dist", NULL, 2480720, 18585, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_file_util", NULL, 2499305, 5667, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_filelist", NULL, 2504972, 9624, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_install", NULL, 2514596, 8860, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_install_data", NULL, 2523456, 2754, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_install_headers", NULL, 2526210, 1976, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_install_lib", NULL, 2528186, 4708, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_install_scripts", NULL, 2532894, 3285, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_msvc9compiler", NULL, 2536179, 6576, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_register", NULL, 2542755, 9779, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_sdist", NULL, 2552534, 15318, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_spawn", NULL, 2567852, 2570, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_sysconfig", NULL, 2570422, 6247, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_text_file", NULL, 2576669, 3089, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_unixccompiler", NULL, 2579758, 7009, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_upload", NULL, 2586767, 5948, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_util", NULL, 2592715, 1828, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_version", NULL, 2594543, 3510, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.tests.test_versionpredicate", NULL, 2598053, 745, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.text_file", NULL, 2598798, 9801, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.unixccompiler", NULL, 2608599, 8209, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.util", NULL, 2616808, 15043, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.version", NULL, 2631851, 7950, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.versionpredicate", NULL, 2639801, 5884, NUITKA_BYTECODE_FLAG },
    { (char *)"doctest", NULL, 2645685, 89732, NUITKA_BYTECODE_FLAG },
    { (char *)"dumbdbm", NULL, 2735417, 7526, NUITKA_BYTECODE_FLAG },
    { (char *)"dummy_thread", NULL, 2742943, 6136, NUITKA_BYTECODE_FLAG },
    { (char *)"dummy_threading", NULL, 2749079, 1327, NUITKA_BYTECODE_FLAG },
    { (char *)"email", NULL, 2750406, 3164, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"email._parseaddr", NULL, 2753570, 15167, NUITKA_BYTECODE_FLAG },
    { (char *)"email.base64mime", NULL, 2768737, 5565, NUITKA_BYTECODE_FLAG },
    { (char *)"email.charset", NULL, 2774302, 14383, NUITKA_BYTECODE_FLAG },
    { (char *)"email.encoders", NULL, 2788685, 2574, NUITKA_BYTECODE_FLAG },
    { (char *)"email.errors", NULL, 2791259, 4271, NUITKA_BYTECODE_FLAG },
    { (char *)"email.feedparser", NULL, 2795530, 12764, NUITKA_BYTECODE_FLAG },
    { (char *)"email.generator", NULL, 2808294, 11322, NUITKA_BYTECODE_FLAG },
    { (char *)"email.header", NULL, 2819616, 14454, NUITKA_BYTECODE_FLAG },
    { (char *)"email.iterators", NULL, 2834070, 2608, NUITKA_BYTECODE_FLAG },
    { (char *)"email.message", NULL, 2836678, 30965, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime", NULL, 2867643, 172, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"email.mime.application", NULL, 2867815, 1726, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime.audio", NULL, 2869541, 3101, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime.base", NULL, 2872642, 1258, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime.image", NULL, 2873900, 2191, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime.message", NULL, 2876091, 1590, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime.multipart", NULL, 2877681, 1811, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime.nonmultipart", NULL, 2879492, 1030, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime.text", NULL, 2880522, 1450, NUITKA_BYTECODE_FLAG },
    { (char *)"email.parser", NULL, 2881972, 4221, NUITKA_BYTECODE_FLAG },
    { (char *)"email.quoprimime", NULL, 2886193, 9492, NUITKA_BYTECODE_FLAG },
    { (char *)"email.test", NULL, 2895685, 172, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"email.test.test_email", NULL, 2895857, 160974, NUITKA_BYTECODE_FLAG },
    { (char *)"email.test.test_email_codecs", NULL, 3056831, 3174, NUITKA_BYTECODE_FLAG },
    { (char *)"email.test.test_email_codecs_renamed", NULL, 3060005, 3222, NUITKA_BYTECODE_FLAG },
    { (char *)"email.test.test_email_renamed", NULL, 3063227, 149726, NUITKA_BYTECODE_FLAG },
    { (char *)"email.test.test_email_torture", NULL, 3212953, 4959, NUITKA_BYTECODE_FLAG },
    { (char *)"email.utils", NULL, 3217912, 9955, NUITKA_BYTECODE_FLAG },
    { (char *)"encodings.mbcs", NULL, 3227867, 2435, NUITKA_BYTECODE_FLAG },
    { (char *)"filecmp", NULL, 3230302, 10614, NUITKA_BYTECODE_FLAG },
    { (char *)"fileinput", NULL, 3240916, 16322, NUITKA_BYTECODE_FLAG },
    { (char *)"fnmatch", NULL, 3257238, 3906, NUITKA_BYTECODE_FLAG },
    { (char *)"formatter", NULL, 3261144, 23020, NUITKA_BYTECODE_FLAG },
    { (char *)"fpformat", NULL, 3284164, 5064, NUITKA_BYTECODE_FLAG },
    { (char *)"fractions", NULL, 3289228, 21675, NUITKA_BYTECODE_FLAG },
    { (char *)"ftplib", NULL, 3310903, 38022, NUITKA_BYTECODE_FLAG },
    { (char *)"genericpath", NULL, 3348925, 4059, NUITKA_BYTECODE_FLAG },
    { (char *)"getopt", NULL, 3352984, 7146, NUITKA_BYTECODE_FLAG },
    { (char *)"getpass", NULL, 3360130, 5086, NUITKA_BYTECODE_FLAG },
    { (char *)"gettext", NULL, 3365216, 17729, NUITKA_BYTECODE_FLAG },
    { (char *)"glob", NULL, 3382945, 3335, NUITKA_BYTECODE_FLAG },
    { (char *)"gzip", NULL, 3386280, 16687, NUITKA_BYTECODE_FLAG },
    { (char *)"hashlib", NULL, 3402967, 7509, NUITKA_BYTECODE_FLAG },
    { (char *)"heapq", NULL, 3410476, 15457, NUITKA_BYTECODE_FLAG },
    { (char *)"hmac", NULL, 3425933, 5034, NUITKA_BYTECODE_FLAG },
    { (char *)"hotshot", NULL, 3430967, 4026, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"hotshot.log", NULL, 3434993, 6235, NUITKA_BYTECODE_FLAG },
    { (char *)"hotshot.stats", NULL, 3441228, 3920, NUITKA_BYTECODE_FLAG },
    { (char *)"hotshot.stones", NULL, 3445148, 1248, NUITKA_BYTECODE_FLAG },
    { (char *)"htmlentitydefs", NULL, 3446396, 6409, NUITKA_BYTECODE_FLAG },
    { (char *)"htmllib", NULL, 3452805, 24851, NUITKA_BYTECODE_FLAG },
    { (char *)"httplib", NULL, 3477656, 40563, NUITKA_BYTECODE_FLAG },
    { (char *)"ihooks", NULL, 3518219, 24814, NUITKA_BYTECODE_FLAG },
    { (char *)"imaplib", NULL, 3543033, 50303, NUITKA_BYTECODE_FLAG },
    { (char *)"imghdr", NULL, 3593336, 5630, NUITKA_BYTECODE_FLAG },
    { (char *)"importlib", NULL, 3598966, 1644, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"imputil", NULL, 3600610, 17515, NUITKA_BYTECODE_FLAG },
    { (char *)"inspect", NULL, 3618125, 43474, NUITKA_BYTECODE_FLAG },
    { (char *)"io", NULL, 3661599, 3830, NUITKA_BYTECODE_FLAG },
    { (char *)"json", NULL, 3665429, 14197, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"json.decoder", NULL, 3679626, 12687, NUITKA_BYTECODE_FLAG },
    { (char *)"json.encoder", NULL, 3692313, 14552, NUITKA_BYTECODE_FLAG },
    { (char *)"json.scanner", NULL, 3706865, 2371, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests", NULL, 3709236, 3955, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"json.tests.test_check_circular", NULL, 3713191, 2853, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_decode", NULL, 3716044, 5085, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_default", NULL, 3721129, 1287, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_dump", NULL, 3722416, 2618, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_encode_basestring_ascii", NULL, 3725034, 3705, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_fail", NULL, 3728739, 3228, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_float", NULL, 3731967, 2839, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_indent", NULL, 3734806, 2796, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_pass1", NULL, 3737602, 2744, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_pass2", NULL, 3740346, 1355, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_pass3", NULL, 3741701, 1450, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_recursion", NULL, 3743151, 5235, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_scanstring", NULL, 3748386, 6675, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_separators", NULL, 3755061, 2108, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_speedups", NULL, 3757169, 2104, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_tool", NULL, 3759273, 3062, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tests.test_unicode", NULL, 3762335, 6390, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tool", NULL, 3768725, 1386, NUITKA_BYTECODE_FLAG },
    { (char *)"keyword", NULL, 3770111, 2197, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3", NULL, 3772308, 169, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"lib2to3.btm_matcher", NULL, 3772477, 6372, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.btm_utils", NULL, 3778849, 8205, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixer_base", NULL, 3787054, 8049, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixer_util", NULL, 3795103, 16375, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes", NULL, 3811478, 175, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"lib2to3.fixes.fix_apply", NULL, 3811653, 2025, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_asserts", NULL, 3813678, 1703, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_basestring", NULL, 3815381, 949, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_buffer", NULL, 3816330, 1106, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_callable", NULL, 3817436, 1649, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_dict", NULL, 3819085, 3961, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_except", NULL, 3823046, 3201, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_exec", NULL, 3826247, 1574, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_execfile", NULL, 3827821, 2215, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_exitfunc", NULL, 3830036, 2999, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_filter", NULL, 3833035, 2412, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_funcattrs", NULL, 3835447, 1270, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_future", NULL, 3836717, 1075, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_getcwdu", NULL, 3837792, 1082, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_has_key", NULL, 3838874, 3340, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_idioms", NULL, 3842214, 4879, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_import", NULL, 3847093, 3545, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_imports", NULL, 3850638, 5872, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_imports2", NULL, 3856510, 726, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_input", NULL, 3857236, 1290, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_intern", NULL, 3858526, 1761, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_isinstance", NULL, 3860287, 1994, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_itertools", NULL, 3862281, 1947, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_itertools_imports", NULL, 3864228, 2172, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_long", NULL, 3866400, 997, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_map", NULL, 3867397, 3196, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_metaclass", NULL, 3870593, 7046, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_methodattrs", NULL, 3877639, 1294, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_ne", NULL, 3878933, 1193, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_next", NULL, 3880126, 3947, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_nonzero", NULL, 3884073, 1242, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_numliterals", NULL, 3885315, 1457, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_operator", NULL, 3886772, 5892, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_paren", NULL, 3892664, 1699, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_print", NULL, 3894363, 2935, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_raise", NULL, 3897298, 2654, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_raw_input", NULL, 3899952, 1092, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_reduce", NULL, 3901044, 1418, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_renames", NULL, 3902462, 2813, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_repr", NULL, 3905275, 1172, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_set_literal", NULL, 3906447, 2196, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_standarderror", NULL, 3908643, 1009, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_sys_exc", NULL, 3909652, 1913, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_throw", NULL, 3911565, 2152, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_tuple_params", NULL, 3913717, 5950, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_types", NULL, 3919667, 2341, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_unicode", NULL, 3922008, 1924, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_urllib", NULL, 3923932, 7602, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_ws_comma", NULL, 3931534, 1538, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_xrange", NULL, 3933072, 3479, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_xreadlines", NULL, 3936551, 1308, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_zip", NULL, 3937859, 1502, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.main", NULL, 3939361, 10323, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.patcomp", NULL, 3949684, 7258, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2", NULL, 3956942, 216, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"lib2to3.pgen2.conv", NULL, 3957158, 8481, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2.driver", NULL, 3965639, 5930, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2.grammar", NULL, 3971569, 6368, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2.literals", NULL, 3977937, 2210, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2.parse", NULL, 3980147, 7828, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2.pgen", NULL, 3987975, 13765, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2.token", NULL, 4001740, 2495, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2.tokenize", NULL, 4004235, 17962, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pygram", NULL, 4022197, 1556, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pytree", NULL, 4023753, 34095, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.refactor", NULL, 4057848, 25999, NUITKA_BYTECODE_FLAG },
    { (char *)"linecache", NULL, 4083847, 3564, NUITKA_BYTECODE_FLAG },
    { (char *)"logging", NULL, 4087411, 63512, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"logging.config", NULL, 4150923, 28538, NUITKA_BYTECODE_FLAG },
    { (char *)"logging.handlers", NULL, 4179461, 42531, NUITKA_BYTECODE_FLAG },
    { (char *)"macpath", NULL, 4221992, 8623, NUITKA_BYTECODE_FLAG },
    { (char *)"macurl2path", NULL, 4230615, 2436, NUITKA_BYTECODE_FLAG },
    { (char *)"mailbox", NULL, 4233051, 87668, NUITKA_BYTECODE_FLAG },
    { (char *)"mailcap", NULL, 4320719, 7721, NUITKA_BYTECODE_FLAG },
    { (char *)"markupbase", NULL, 4328440, 10152, NUITKA_BYTECODE_FLAG },
    { (char *)"md5", NULL, 4338592, 420, NUITKA_BYTECODE_FLAG },
    { (char *)"mhlib", NULL, 4339012, 37709, NUITKA_BYTECODE_FLAG },
    { (char *)"mimetools", NULL, 4376721, 9216, NUITKA_BYTECODE_FLAG },
    { (char *)"mimetypes", NULL, 4385937, 19241, NUITKA_BYTECODE_FLAG },
    { (char *)"mimify", NULL, 4405178, 12743, NUITKA_BYTECODE_FLAG },
    { (char *)"modulefinder", NULL, 4417921, 20453, NUITKA_BYTECODE_FLAG },
    { (char *)"multifile", NULL, 4438374, 6162, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing", NULL, 4444536, 9757, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"multiprocessing.connection", NULL, 4454293, 16174, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.dummy", NULL, 4470467, 6452, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"multiprocessing.dummy.connection", NULL, 4476919, 3356, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.forking", NULL, 4480275, 16004, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.heap", NULL, 4496279, 7901, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.managers", NULL, 4504180, 43947, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.pool", NULL, 4548127, 25383, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.process", NULL, 4573510, 10787, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.queues", NULL, 4584297, 13286, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.reduction", NULL, 4597583, 6856, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.sharedctypes", NULL, 4604439, 9924, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.synchronize", NULL, 4614363, 13016, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.util", NULL, 4627379, 11522, NUITKA_BYTECODE_FLAG },
    { (char *)"mutex", NULL, 4638901, 2857, NUITKA_BYTECODE_FLAG },
    { (char *)"netrc", NULL, 4641758, 5114, NUITKA_BYTECODE_FLAG },
    { (char *)"new", NULL, 4646872, 904, NUITKA_BYTECODE_FLAG },
    { (char *)"nntplib", NULL, 4647776, 23236, NUITKA_BYTECODE_FLAG },
    { (char *)"ntpath", NULL, 4671012, 14222, NUITKA_BYTECODE_FLAG },
    { (char *)"nturl2path", NULL, 4685234, 1957, NUITKA_BYTECODE_FLAG },
    { (char *)"numbers", NULL, 4687191, 17104, NUITKA_BYTECODE_FLAG },
    { (char *)"opcode", NULL, 4704295, 6394, NUITKA_BYTECODE_FLAG },
    { (char *)"optparse", NULL, 4710689, 61123, NUITKA_BYTECODE_FLAG },
    { (char *)"os", NULL, 4771812, 28341, NUITKA_BYTECODE_FLAG },
    { (char *)"os2emxpath", NULL, 4800153, 4968, NUITKA_BYTECODE_FLAG },
    { (char *)"pdb", NULL, 4805121, 49300, NUITKA_BYTECODE_FLAG },
    { (char *)"pickle", NULL, 4854421, 43662, NUITKA_BYTECODE_FLAG },
    { (char *)"pickletools", NULL, 4898083, 58736, NUITKA_BYTECODE_FLAG },
    { (char *)"pipes", NULL, 4956819, 10100, NUITKA_BYTECODE_FLAG },
    { (char *)"pkgutil", NULL, 4966919, 20575, NUITKA_BYTECODE_FLAG },
    { (char *)"platform", NULL, 4987494, 39322, NUITKA_BYTECODE_FLAG },
    { (char *)"plistlib", NULL, 5026816, 22211, NUITKA_BYTECODE_FLAG },
    { (char *)"popen2", NULL, 5049027, 9817, NUITKA_BYTECODE_FLAG },
    { (char *)"poplib", NULL, 5058844, 14986, NUITKA_BYTECODE_FLAG },
    { (char *)"posixfile", NULL, 5073830, 8244, NUITKA_BYTECODE_FLAG },
    { (char *)"posixpath", NULL, 5082074, 12562, NUITKA_BYTECODE_FLAG },
    { (char *)"pprint", NULL, 5094636, 11136, NUITKA_BYTECODE_FLAG },
    { (char *)"profile", NULL, 5105772, 18400, NUITKA_BYTECODE_FLAG },
    { (char *)"pstats", NULL, 5124172, 28005, NUITKA_BYTECODE_FLAG },
    { (char *)"pty", NULL, 5152177, 5458, NUITKA_BYTECODE_FLAG },
    { (char *)"py_compile", NULL, 5157635, 6770, NUITKA_BYTECODE_FLAG },
    { (char *)"pyclbr", NULL, 5164405, 10293, NUITKA_BYTECODE_FLAG },
    { (char *)"pydoc", NULL, 5174698, 102182, NUITKA_BYTECODE_FLAG },
    { (char *)"pydoc_data", NULL, 5276880, 172, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"pydoc_data.topics", NULL, 5277052, 396026, NUITKA_BYTECODE_FLAG },
    { (char *)"random", NULL, 5673078, 27712, NUITKA_BYTECODE_FLAG },
    { (char *)"repr", NULL, 5700790, 6227, NUITKA_BYTECODE_FLAG },
    { (char *)"rexec", NULL, 5707017, 27136, NUITKA_BYTECODE_FLAG },
    { (char *)"rfc822", NULL, 5734153, 34842, NUITKA_BYTECODE_FLAG },
    { (char *)"rlcompleter", NULL, 5768995, 6455, NUITKA_BYTECODE_FLAG },
    { (char *)"robotparser", NULL, 5775450, 9152, NUITKA_BYTECODE_FLAG },
    { (char *)"runpy", NULL, 5784602, 9345, NUITKA_BYTECODE_FLAG },
    { (char *)"sched", NULL, 5793947, 5436, NUITKA_BYTECODE_FLAG },
    { (char *)"sets", NULL, 5799383, 19687, NUITKA_BYTECODE_FLAG },
    { (char *)"sgmllib", NULL, 5819070, 17778, NUITKA_BYTECODE_FLAG },
    { (char *)"sha", NULL, 5836848, 463, NUITKA_BYTECODE_FLAG },
    { (char *)"shelve", NULL, 5837311, 11598, NUITKA_BYTECODE_FLAG },
    { (char *)"shlex", NULL, 5848909, 8185, NUITKA_BYTECODE_FLAG },
    { (char *)"shutil", NULL, 5857094, 19910, NUITKA_BYTECODE_FLAG },
    { (char *)"site", NULL, 5877004, 18769, NUITKA_BYTECODE_FLAG },
    { (char *)"smtpd", NULL, 5895773, 17438, NUITKA_BYTECODE_FLAG },
    { (char *)"smtplib", NULL, 5913211, 33007, NUITKA_BYTECODE_FLAG },
    { (char *)"sndhdr", NULL, 5946218, 8199, NUITKA_BYTECODE_FLAG },
    { (char *)"socket", NULL, 5954417, 17645, NUITKA_BYTECODE_FLAG },
    { (char *)"sqlite3", NULL, 5972062, 206, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"sqlite3.dbapi2", NULL, 5972268, 3150, NUITKA_BYTECODE_FLAG },
    { (char *)"sqlite3.dump", NULL, 5975418, 2213, NUITKA_BYTECODE_FLAG },
    { (char *)"sre", NULL, 5977631, 561, NUITKA_BYTECODE_FLAG },
    { (char *)"ssl", NULL, 5978192, 34756, NUITKA_BYTECODE_FLAG },
    { (char *)"stat", NULL, 6012948, 3243, NUITKA_BYTECODE_FLAG },
    { (char *)"statvfs", NULL, 6016191, 662, NUITKA_BYTECODE_FLAG },
    { (char *)"string", NULL, 6016853, 23001, NUITKA_BYTECODE_FLAG },
    { (char *)"stringold", NULL, 6039854, 13891, NUITKA_BYTECODE_FLAG },
    { (char *)"subprocess", NULL, 6053745, 44467, NUITKA_BYTECODE_FLAG },
    { (char *)"sunau", NULL, 6098212, 20797, NUITKA_BYTECODE_FLAG },
    { (char *)"sunaudio", NULL, 6119009, 2229, NUITKA_BYTECODE_FLAG },
    { (char *)"symbol", NULL, 6121238, 3118, NUITKA_BYTECODE_FLAG },
    { (char *)"symtable", NULL, 6124356, 14278, NUITKA_BYTECODE_FLAG },
    { (char *)"sysconfig", NULL, 6138634, 18837, NUITKA_BYTECODE_FLAG },
    { (char *)"tabnanny", NULL, 6157471, 9235, NUITKA_BYTECODE_FLAG },
    { (char *)"tarfile", NULL, 6166706, 84000, NUITKA_BYTECODE_FLAG },
    { (char *)"telnetlib", NULL, 6250706, 24789, NUITKA_BYTECODE_FLAG },
    { (char *)"tempfile", NULL, 6275495, 22684, NUITKA_BYTECODE_FLAG },
    { (char *)"test", NULL, 6298179, 166, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"test.pystone", NULL, 6298345, 9021, NUITKA_BYTECODE_FLAG },
    { (char *)"test.script_helper", NULL, 6307366, 6479, NUITKA_BYTECODE_FLAG },
    { (char *)"test.test_support", NULL, 6313845, 59984, NUITKA_BYTECODE_FLAG },
    { (char *)"textwrap", NULL, 6373829, 12596, NUITKA_BYTECODE_FLAG },
    { (char *)"this", NULL, 6386425, 1262, NUITKA_BYTECODE_FLAG },
    { (char *)"threading", NULL, 6387687, 47759, NUITKA_BYTECODE_FLAG },
    { (char *)"timeit", NULL, 6435446, 12767, NUITKA_BYTECODE_FLAG },
    { (char *)"tkColorChooser", NULL, 6448213, 1662, NUITKA_BYTECODE_FLAG },
    { (char *)"tkCommonDialog", NULL, 6449875, 1808, NUITKA_BYTECODE_FLAG },
    { (char *)"tkFileDialog", NULL, 6451683, 5953, NUITKA_BYTECODE_FLAG },
    { (char *)"tkFont", NULL, 6457636, 8116, NUITKA_BYTECODE_FLAG },
    { (char *)"tkMessageBox", NULL, 6465752, 4436, NUITKA_BYTECODE_FLAG },
    { (char *)"tkSimpleDialog", NULL, 6470188, 10405, NUITKA_BYTECODE_FLAG },
    { (char *)"toaiff", NULL, 6480593, 3298, NUITKA_BYTECODE_FLAG },
    { (char *)"token", NULL, 6483891, 4058, NUITKA_BYTECODE_FLAG },
    { (char *)"tokenize", NULL, 6487949, 15015, NUITKA_BYTECODE_FLAG },
    { (char *)"trace", NULL, 6502964, 24484, NUITKA_BYTECODE_FLAG },
    { (char *)"traceback", NULL, 6527448, 12671, NUITKA_BYTECODE_FLAG },
    { (char *)"ttk", NULL, 6540119, 69369, NUITKA_BYTECODE_FLAG },
    { (char *)"tty", NULL, 6609488, 1459, NUITKA_BYTECODE_FLAG },
    { (char *)"turtle", NULL, 6610947, 151072, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest", NULL, 6762019, 3006, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"unittest.case", NULL, 6765025, 45069, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest.loader", NULL, 6810094, 12472, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest.main", NULL, 6822566, 8453, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest.result", NULL, 6831019, 9116, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest.runner", NULL, 6840135, 8691, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest.signals", NULL, 6848826, 3258, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest.suite", NULL, 6852084, 12280, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest.util", NULL, 6864364, 4910, NUITKA_BYTECODE_FLAG },
    { (char *)"urllib", NULL, 6869274, 55630, NUITKA_BYTECODE_FLAG },
    { (char *)"urllib2", NULL, 6924904, 53592, NUITKA_BYTECODE_FLAG },
    { (char *)"urlparse", NULL, 6978496, 15809, NUITKA_BYTECODE_FLAG },
    { (char *)"user", NULL, 6994305, 1766, NUITKA_BYTECODE_FLAG },
    { (char *)"uu", NULL, 6996071, 4554, NUITKA_BYTECODE_FLAG },
    { (char *)"uuid", NULL, 7000625, 24954, NUITKA_BYTECODE_FLAG },
    { (char *)"warnings", NULL, 7025579, 14330, NUITKA_BYTECODE_FLAG },
    { (char *)"wave", NULL, 7039909, 22563, NUITKA_BYTECODE_FLAG },
    { (char *)"weakref", NULL, 7062472, 18004, NUITKA_BYTECODE_FLAG },
    { (char *)"webbrowser", NULL, 7080476, 22021, NUITKA_BYTECODE_FLAG },
    { (char *)"whichdb", NULL, 7102497, 2333, NUITKA_BYTECODE_FLAG },
    { (char *)"wsgiref", NULL, 7104830, 771, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"wsgiref.handlers", NULL, 7105601, 18096, NUITKA_BYTECODE_FLAG },
    { (char *)"wsgiref.headers", NULL, 7123697, 8365, NUITKA_BYTECODE_FLAG },
    { (char *)"wsgiref.simple_server", NULL, 7132062, 6927, NUITKA_BYTECODE_FLAG },
    { (char *)"wsgiref.util", NULL, 7138989, 6585, NUITKA_BYTECODE_FLAG },
    { (char *)"wsgiref.validate", NULL, 7145574, 18799, NUITKA_BYTECODE_FLAG },
    { (char *)"xdrlib", NULL, 7164373, 11994, NUITKA_BYTECODE_FLAG },
    { (char *)"xml", NULL, 7176367, 1120, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"xml.dom", NULL, 7177487, 7571, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"xml.dom.NodeFilter", NULL, 7185058, 1268, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.dom.domreg", NULL, 7186326, 3553, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.dom.expatbuilder", NULL, 7189879, 37631, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.dom.minicompat", NULL, 7227510, 4301, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.dom.minidom", NULL, 7231811, 77083, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.dom.pulldom", NULL, 7308894, 15170, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.dom.xmlbuilder", NULL, 7324064, 19256, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.etree", NULL, 7343320, 171, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"xml.etree.ElementInclude", NULL, 7343491, 2159, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.etree.ElementPath", NULL, 7345650, 8860, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.etree.ElementTree", NULL, 7354510, 40456, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.etree.cElementTree", NULL, 7394966, 218, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.parsers", NULL, 7395184, 356, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"xml.parsers.expat", NULL, 7395540, 329, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.sax", NULL, 7395869, 3991, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"xml.sax._exceptions", NULL, 7399860, 7011, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.sax.expatreader", NULL, 7406871, 16833, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.sax.handler", NULL, 7423704, 14270, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.sax.saxutils", NULL, 7437974, 17493, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.sax.xmlreader", NULL, 7455467, 22614, NUITKA_BYTECODE_FLAG },
    { (char *)"xmllib", NULL, 7478081, 29540, NUITKA_BYTECODE_FLAG },
    { (char *)"xmlrpclib", NULL, 7507621, 50807, NUITKA_BYTECODE_FLAG },
    { (char *)"zipfile", NULL, 7558428, 44687, NUITKA_BYTECODE_FLAG },
    { NULL, NULL, 0, 0, 0 }
};

void setupMetaPathBasedLoader( void )
{
    static bool init_done = false;

    if ( init_done == false )
    {
        registerMetaPathBasedUnfreezer( meta_path_loader_entries );
        init_done = true;
    }
}
