/* -----------------------------------------------------------------------------
*  CArgumentParser
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CArgumentParser.hpp"

#include "stdlib.h"
#include "CArray.hpp"
#include "CArrayString.hpp"
#include "CFileName.hpp"
#include "CString.hpp"
#include "CLogTxt.hpp"
#include "asrtbas.h"

enum prv_valueTypes_t
{
    NO_TYPE,
    FLAG,
    CHAR,
    STR,
    INT,
    ULONG,
    DOUBLE,
    DESCRIPTION
};

enum prv_errorTypes_t
{
    NO_ERROR,
    HELP_ERROR,
    NOT_FOUND_ERROR,
    REPEATED_ERROR,
    PROCESSING_ERROR,
    NOT_ENOUGH_PARAMETERS,
    UNKNOWN_OPTION
};

struct PrvParameter
{
    PrvParameter(const char* option, bool required,
                 const char* description, enum prv_valueTypes_t type,
                 bool showDefault);
    ~PrvParameter();

    char* option;
    bool required;
    char* description;
    bool showDefault;
    enum prv_valueTypes_t type;

    bool processed;

    bool valueFlag;
    char valueChar;
    char* valueStr;
    int valueInt;
    unsigned long valueULong;
    double valueDouble;
};

struct prv_argumentParser_t
{
    bool parsed;
    char* programDescription;
    class CArray<PrvParameter> *parameterList;
};

//------------------------------------------------------------------------------
//
PrvParameter::PrvParameter(const char* option, bool required,
                           const char* description, enum prv_valueTypes_t type,
                           bool showDefault)
{
    this->option = CString::copy( option );
    this->required = required;
    this->description = CString::copy( description );
    this->type = type;
    this->showDefault = showDefault;

    this->processed = false;

    this->valueFlag = false;
    this->valueChar = '\0';
    this->valueStr = CString::copy( "" );
    this->valueInt = 0;
    this->valueULong = 0;
    this->valueDouble = 0.0;
}

//------------------------------------------------------------------------------
//
PrvParameter::~PrvParameter()
{
    delete option;
    delete description;
    CString::free( &valueStr );
}

//------------------------------------------------------------------------------
//
static struct prv_argumentParser_t* prv_create(
                                const char* programDescription,
                                class CArray<PrvParameter> *parameterList)
{
    struct prv_argumentParser_t *data;

    assert_no_null( parameterList );

    data = new prv_argumentParser_t;
    assert_no_null( data );

    data->parsed = false;
    data->programDescription = CString::copy( programDescription );
    data->parameterList = parameterList;

    return data;
}

//------------------------------------------------------------------------------
//
CArgumentParser :: CArgumentParser( const char* programDescription )
{
    class CArray<PrvParameter> *parameterList;

    assert_no_null( programDescription );

    parameterList = new CArray<PrvParameter>();

    m_data = prv_create( programDescription, parameterList );
}

//------------------------------------------------------------------------------
//
CArgumentParser :: ~CArgumentParser()
{
    assert_no_null( m_data );

    delete m_data->parameterList;

    CString::free( &m_data->programDescription );

    delete m_data;
}

//------------------------------------------------------------------------------
//
static bool prv_funcSearch( const class PrvParameter *parameter, const char* option )
{
    return CString::equal( parameter->option, option );
}

//------------------------------------------------------------------------------
//
static class PrvParameter* prv_addArg(
                                    bool isParsed,
                                    const char* option, bool required,
                                    const char* description, enum prv_valueTypes_t type,
                                    bool showDefault,
                                    class CArray<PrvParameter> *parameterList )
{
    class PrvParameter *parameter;

    assert_with_message( isParsed == false, "Can not add parameters after parsing" );

    assert( type == DESCRIPTION || 
            parameterList->exist<const char>( option, prv_funcSearch, NULL ) == false );

    parameter = new PrvParameter( option, required, description, type, showDefault );

    parameterList->add( parameter );

    return parameter;
}

//------------------------------------------------------------------------------
//
void CArgumentParser :: addArgFlag( const char* option, const char* description )
{
    prv_addArg( m_data->parsed, option, false, description, FLAG, false,
                m_data->parameterList );
}

//------------------------------------------------------------------------------
//
void CArgumentParser :: addArgChar( const char* option, const char* description )
{
    prv_addArg( m_data->parsed, option, true, description, CHAR, false,
                m_data->parameterList );
}

//------------------------------------------------------------------------------
//
void CArgumentParser :: addArgChar_optional( const char* option, const char* description,
                                             char defaultChar, bool showDefault )
{
    class PrvParameter *parameter;

    parameter = prv_addArg( m_data->parsed, option, false, description, CHAR, showDefault,
                            m_data->parameterList );

    parameter->valueChar = defaultChar;
}

//------------------------------------------------------------------------------
//
void CArgumentParser :: addArgStr( const char* option, const char* description )
{
    prv_addArg( m_data->parsed, option, true, description, STR, false,
                m_data->parameterList );
}

//------------------------------------------------------------------------------
//
void CArgumentParser :: addArgStr_optional( const char* option, const char* description,
                                            const char* defaultStr, bool showDefault )
{
    class PrvParameter *parameter;

    assert_no_null( defaultStr );

    parameter = prv_addArg( m_data->parsed, option, false, description, STR, showDefault,
                            m_data->parameterList );

    CString::free( &parameter->valueStr );
    parameter->valueStr = CString::copy( defaultStr );
}

//------------------------------------------------------------------------------
//
void CArgumentParser :: addArgInt( const char* option, const char* description )
{
    prv_addArg( m_data->parsed, option, true, description, INT, false,
                m_data->parameterList );
}

//------------------------------------------------------------------------------
//
void CArgumentParser :: addArgInt_optional( const char* option, const char* description,
                                            int defaultValue, bool showDefault )
{
    class PrvParameter *parameter;

    parameter = prv_addArg( m_data->parsed, option, false, description, INT, showDefault,
                            m_data->parameterList );

    parameter->valueInt = defaultValue;
}


//------------------------------------------------------------------------------
//
void CArgumentParser :: addArgULong( const char* option, const char* description )
{
    prv_addArg( m_data->parsed, option, true, description, ULONG, false,
                m_data->parameterList );
}

//------------------------------------------------------------------------------
//
void CArgumentParser :: addArgULong_optional( const char* option, const char* description,
                                            unsigned long defaultValue, bool showDefault )
{
    class PrvParameter *parameter;

    parameter = prv_addArg( m_data->parsed, option, false, description, ULONG, showDefault,
                            m_data->parameterList );

    parameter->valueULong = defaultValue;
}

//------------------------------------------------------------------------------
//
void CArgumentParser :: addArgDouble( const char* option, const char* description )
{
    prv_addArg( m_data->parsed, option, true, description, DOUBLE, false,
                m_data->parameterList );
}

//------------------------------------------------------------------------------
//
void CArgumentParser :: addArgDouble_optional( const char* option, const char* description,
                                            double defaultValue, bool showDefault )
{
    class PrvParameter *parameter;

    parameter = prv_addArg( m_data->parsed, option, false, description, DOUBLE, showDefault,
                            m_data->parameterList );

    parameter->valueDouble = defaultValue;
}

//------------------------------------------------------------------------------
//
void CArgumentParser :: addDescription( const char* description )
{
    prv_addArg( m_data->parsed, "", false, description, DESCRIPTION, false,
                m_data->parameterList );
}

//------------------------------------------------------------------------------
//
static const char* prv_printType( enum prv_valueTypes_t type )
{
    switch( type )
    {
        case STR:
                return "string";
                break;
        case CHAR:
                return "char";
                break;
        case INT:
                return "int";
                break;
        case ULONG:
                return "ulong";
                break;
        case DOUBLE:
                return "double";
                break;

        default_message( "Unknown type" );
    }

    return NULL;
}

//------------------------------------------------------------------------------
//
static char* prv_printDefaultValue( const class PrvParameter *parameter )
{
    char *valueString;

    switch(parameter->type)
    {
        case STR:
            valueString = CString::copy( parameter->valueStr );
            break;
        case CHAR:
            valueString = CString::printf("%c", parameter->valueChar);
            break;
        case INT:
            valueString = CString::printf("%d", parameter->valueInt);
            break;
        case ULONG:
            valueString = CString::printf("%u", parameter->valueULong);
            break;
        case DOUBLE:
            valueString = CString::printf("%g", parameter->valueDouble );
            break;
        default_message( "Unknown type" );
    }

    return valueString;
}

//------------------------------------------------------------------------------
//
static void prv_printError(enum prv_errorTypes_t type, const char* error_args, class ILog *log)
{
    assert_no_null( error_args );
    assert_no_null( log );

    //log->line( "********************************************************************************" );
    //log->line( "*" );

    switch( type )
    {
        case NOT_FOUND_ERROR:
            log->error( "Not found the required option '%s'", error_args );
            break;

        case REPEATED_ERROR:
            log->error( "Repeated option '%s'", error_args );
            break;

        case PROCESSING_ERROR:
            log->error( "Wrong parameter format for option '%s'", error_args );
            break;

        case NOT_ENOUGH_PARAMETERS:
            log->error( "Option '%s' requires an argument", error_args );
            break;

        case UNKNOWN_OPTION:
            log->error( "Unknown option '%s'", error_args );
            break;

        default_message( "Unknown error type" );
    }

    //log->line( "*" );
}

//------------------------------------------------------------------------------
//
static void prv_printDescription( const char* strDescription, class ILog *log )
{
    unsigned long size;
    unsigned long length = 78;

    size = CString::size( strDescription );

    for( unsigned long pos = 0; pos < size; pos += length )
    {
        char* strAux;

        if( pos + length > size )
            length = size - pos;

        strAux = CString::substring( strDescription, pos, length - 1 );

        log->line( "* %s", strAux );

        CString::free( &strAux );
    }
}

//------------------------------------------------------------------------------
//
static void prv_printHelp( const char* programName,
                            const char* programDescription,
                            enum prv_errorTypes_t error,
                            const char* error_args,
                            const class CArray<PrvParameter> *parameterList,
                            class ILog *log )
{
    unsigned long numParameters;

    assert_no_null( log );

    numParameters = parameterList->size();

    if( error != NO_ERROR && error != HELP_ERROR )
    {
        prv_printError( error, error_args, log );
    }

    log->line( "********************************************************************************" );
    log->line( "* %s", programName );

    prv_printDescription( programDescription, log );

    log->line( "* " );
    log->line( "* Usage: %s <options> [optional]:", programName );

    for( unsigned long ind = 0; ind < numParameters; ++ind )
    {
        const class PrvParameter *parameter;

        parameter = parameterList->get( ind );

        if( parameter->type == DESCRIPTION )
        {
            log->line( "* %s", parameter->description );
        }
        else
        {
            char *str;

            str = CString::copy( "*  " );
            if( parameter->required == false )
                CString::concatenate( &str, "[" );

            CString::concatenate( &str, parameter->option );

            if( parameter->type != FLAG )
            {
                CString::concatenate( &str, " <" );
                CString::concatenate( &str, prv_printType( parameter->type ) );
                CString::concatenate( &str, ">" );
            }

            if( parameter->required == false )
                CString::concatenate( &str, "]" );

            CString::concatenate( &str, " " );
            CString::concatenate( &str, parameter->description );

            if( parameter->showDefault == true )
            {
                char* defaultValue;

                defaultValue = prv_printDefaultValue( parameter );

                CString::concatenate( &str, ". Default: " );
                CString::concatenate( &str, defaultValue );

                delete defaultValue;
            }

            log->line( str );

            delete str;
        }
    }

    log->line( "********************************************************************************" );
    log->line( "" );
}

//------------------------------------------------------------------------------
//
static enum prv_errorTypes_t prv_parseParameterData(
                                unsigned long ind,
                                class CArrayString *inputValuesList,
                                class PrvParameter *parameter )
{
    assert_no_null(parameter);
    assert(parameter->type != DESCRIPTION);
    assert(inputValuesList->size() > 0);

    if( parameter->type != FLAG &&
       inputValuesList->size() <= ind + 1 )
    {
        return NOT_ENOUGH_PARAMETERS;
    }

    switch(parameter->type)
    {
        case FLAG:
        {
            parameter->valueFlag = true;
            break;
        }
        case CHAR:
        {
            const char *str;

            str = inputValuesList->get(ind + 1);

            if( CString::size( str ) != 1 )
                return PROCESSING_ERROR;

            parameter->valueChar = str[0];
            break;
        }
        case STR:
        {
            CString::free( &parameter->valueStr );
            parameter->valueStr = CString::copy( inputValuesList->get( ind + 1 ) );
            break;
        }
        case INT:
        {
            int numRead;

            numRead = CString::scanf(inputValuesList->get( ind + 1 ), "%d", &parameter->valueInt);
            if (numRead != 1)
                return PROCESSING_ERROR;

            break;
        }
        case ULONG:
        {
            int numRead;

            numRead = CString::scanf(inputValuesList->get( ind + 1 ), "%u", &parameter->valueULong );
            if (numRead != 1)
                return PROCESSING_ERROR;

            break;
        }
        case DOUBLE:
        {
            int numRead;

            numRead = CString::scanf(inputValuesList->get( ind + 1 ), "%f", &parameter->valueDouble );
            if (numRead != 1)
                return PROCESSING_ERROR;

            break;
        }

        default: assert_message( "Unknown type" );
    }

    return NO_ERROR;
}

//------------------------------------------------------------------------------
//
static enum prv_errorTypes_t prv_parseParameter(
                                const char* inputValue,
                                const class CArray<PrvParameter> *parameterList,
                                class PrvParameter **parameter,
                                char** str_error )
{
    unsigned long numParameters;

    assert_no_null( *parameter );

    numParameters = parameterList->size();
    *parameter = NULL;

    for( unsigned long ind = 0; ind < numParameters; ++ind )
    {
        *parameter = parameterList->get( ind );

        if( (*parameter)->type == DESCRIPTION )
            continue;

        if( CString::equal( inputValue, (*parameter)->option ) == true )
        {
            if( (*parameter)->processed == true )
            {
                *str_error = CString::copy( (*parameter)->option );

                return REPEATED_ERROR;
            }

            (*parameter)->processed = true;

            return NO_ERROR;
        }
    }

    *str_error = CString::copy( inputValue );

    return UNKNOWN_OPTION;
}

//------------------------------------------------------------------------------
//
static enum prv_errorTypes_t prv_parseInputValues(
                                class CArrayString *inputValuesList,
                                const class CArray<PrvParameter> *parameterList,
                                char** str_error )
{
    unsigned long numInputValues;

    assert_no_null( str_error );
    assert( *str_error == NULL );

    numInputValues = inputValuesList->size();

    for( unsigned long ind = 0; ind < numInputValues; ++ind )
    {
        class PrvParameter *parameter;
        enum prv_errorTypes_t error;

        const char* arg = inputValuesList->get( ind );

        error = prv_parseParameter( arg, parameterList, &parameter, str_error );
        if( NO_ERROR != error )
        {
            return error;
        }

        error = prv_parseParameterData( ind, inputValuesList, parameter );
        if( NO_ERROR != error )
        {
            *str_error = CString::copy( parameter->option );

            return error;
        }

        if( parameter->type != FLAG )
            ++ind;
    }

    return NO_ERROR;
}

//------------------------------------------------------------------------------
//
static enum prv_errorTypes_t prv_parseIfRemainingParameters(
                                const class CArray<PrvParameter> *parameterList,
                                char** str_error )
{
    unsigned long numRemaining;

    assert_no_null( str_error );
    assert( *str_error == NULL );

    numRemaining = parameterList->size();

    for( unsigned long ind = 0; ind < numRemaining; ++ind )
    {
        const class PrvParameter *parameter;

        parameter = parameterList->get( ind );

        if( parameter->processed == false && parameter->required == true )
        {
            *str_error = CString::copy( parameter->option );

            return NOT_FOUND_ERROR;
        }
    }

    return NO_ERROR;
}

//------------------------------------------------------------------------------
//
static enum prv_errorTypes_t prv_parseIfHelpOption( const class CArrayString *inputValuesList )
{
    class CArrayString *helpArgumentList;
    unsigned long numInputValues;

    numInputValues = inputValuesList->size();

    helpArgumentList = new CArrayString();
    helpArgumentList->add( "-help" );
    helpArgumentList->add( "--help" );
    helpArgumentList->add( "-h" );
    helpArgumentList->add( "--h" );

    if( numInputValues == 0 ||
        ( numInputValues == 1 &&
          helpArgumentList->searchString( inputValuesList->get(0), NULL ) == true ) )
    {
        return HELP_ERROR;
    }

    delete helpArgumentList;

    return NO_ERROR;
}

//------------------------------------------------------------------------------
//
static enum prv_errorTypes_t prv_parse(
                            class CArrayString *inputValuesList,
                            const class CArray<PrvParameter> *parameterList,
                            char** str_error )
{
    enum prv_errorTypes_t error;

    assert_no_null( str_error );
    assert( *str_error == NULL );

    error = prv_parseIfHelpOption( inputValuesList );
    if( NO_ERROR != error )
    {
        return error;
    }

    error = prv_parseInputValues( inputValuesList, parameterList, str_error );
    if( NO_ERROR != error )
    {
        return error;
    }

    error = prv_parseIfRemainingParameters( parameterList, str_error );
    if( NO_ERROR != error )
    {
        return error;
    }

    return NO_ERROR;
}

//------------------------------------------------------------------------------
//
bool CArgumentParser :: validateParse( int argc, const char *argv[], class ILog *log_optional )
{
    class CArrayString *inputValuesList;
    char *error_args, *programName;
    enum prv_errorTypes_t error;
    unsigned long numArguments;

    assert( argc >= 0 );
    assert_no_null( argv );
    assert_with_message( m_data->parameterList->size() > 0,
                         "Add arguments before calling this function" );
    assert_with_message( m_data->parsed == false,
                         "This function has already been processed" );


    programName = CFileName::getFileNameWithoutExtension( argv[0] );

    error_args = NULL;

    numArguments = argc - 1;
    inputValuesList = new CArrayString( numArguments );

    for( unsigned long ind = 0; ind < numArguments; ++ind )
    {
        inputValuesList->set( ind, argv[ ind + 1 ] );
    }


    if( NO_ERROR != ( error = prv_parse( inputValuesList, m_data->parameterList, &error_args ) ) )
    {
        class ILog *log;

        if( log_optional == NULL )
            log = new CLogTxt();
        else
            log = log_optional;

        prv_printHelp( programName,
                       m_data->programDescription,
                       error,
                       error_args,
                       m_data->parameterList,
                       log );

        if( log_optional == NULL )
            delete log;
        delete inputValuesList;

        return false;
    }

    m_data->parsed = true;

    delete inputValuesList;
    CString::free( &programName );

    return true;
}

//------------------------------------------------------------------------------
//
static const class PrvParameter* prv_getArg(
                                    bool isParsed,
                                    const char* option,
                                    enum prv_valueTypes_t expected_type,
                                    class CArray<PrvParameter> *parameterList )
{
    const class PrvParameter *parameter;
    unsigned long ind;
    bool status;

    assert_no_null( option );
    assert_with_message( isParsed == true, 
                         "The parsing function has not been called or there was an error when parsing" );

    status = parameterList->existOnlyOneElement<const char>( option, prv_funcSearch, &ind );
    assert_with_message( status == true, "Parameter not found" );

    parameter = parameterList->get( ind );

    assert_with_message( expected_type == NO_TYPE ||
                         expected_type == parameter->type,
                         "Error in parameter data type" );

    return parameter;
}


//------------------------------------------------------------------------------
//
bool CArgumentParser :: isProcessed( const char* option ) const
{
    const class PrvParameter *parameter;

    parameter = prv_getArg( m_data->parsed, option, NO_TYPE, m_data->parameterList );

    return parameter->processed;
}

//------------------------------------------------------------------------------
//
bool CArgumentParser :: getArgFlag( const char* option ) const
{
    const class PrvParameter *parameter;

    parameter = prv_getArg( m_data->parsed, option, FLAG, m_data->parameterList );

    return parameter->valueFlag;
}

//------------------------------------------------------------------------------
//
char CArgumentParser :: getArgChar( const char* option ) const
{
    const class PrvParameter *parameter;

    parameter = prv_getArg( m_data->parsed, option, CHAR, m_data->parameterList );

    return parameter->valueChar;
}

//------------------------------------------------------------------------------
//
char* CArgumentParser :: getArgStr( const char* option ) const
{
    const class PrvParameter *parameter;

    parameter = prv_getArg( m_data->parsed, option, STR, m_data->parameterList );

    return CString::copy( parameter->valueStr );
}

//------------------------------------------------------------------------------
//
int CArgumentParser :: getArgInt( const char* option ) const
{
    const class PrvParameter *parameter;

    parameter = prv_getArg( m_data->parsed, option, INT, m_data->parameterList );

    return parameter->valueInt;
}

//------------------------------------------------------------------------------
//
unsigned long CArgumentParser :: getArgULong( const char* option ) const
{
    const class PrvParameter *parameter;

    parameter = prv_getArg( m_data->parsed, option, ULONG, m_data->parameterList );

    return parameter->valueULong;
}

//------------------------------------------------------------------------------
//
double CArgumentParser :: getArgDouble( const char* option ) const
{
    const class PrvParameter *parameter;

    parameter = prv_getArg( m_data->parsed, option, DOUBLE, m_data->parameterList );

    return  parameter->valueDouble;
}
