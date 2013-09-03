#include "CProbabilityTable.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CMath.hpp"
#include "math.h"

static const double prv_TOLERANCE = 0.000001;
static const unsigned long prv_NUM_ROW_MAX = 10;
static const unsigned long prv_NUM_COL_MAX = 10;
static const double prv_PROBABILITY_GROWNING_TREE_DEFAULT = 0.3; 
static const double prv_PROBABILITY_STORM_DEFAULT = 0.02;

struct prv_dataProbability_t
{
    unsigned long numRows, numCols;
    
    unsigned long row_current, col_current;
    
    double **probabilityMatrix;
};

struct prv_dataPrivate_t
{
    struct prv_dataProbability_t *probabilityGrowing;
    struct prv_dataProbability_t *probabilityStorm;
};

//---------------------------------------------------------------

static double *prv_createRow(unsigned long numCols, double valueDefault)
{
    double *row;
    
    row = new double[numCols];

    for (unsigned long i = 0; i < numCols; i++)
        row[i] = valueDefault;
        
    return row;
}

//---------------------------------------------------------------

static double **prv_createMatrix(unsigned long numRows, unsigned long numCols, double valueDefault)
{
    double **matrix;
    
    matrix = new double*[numRows];
    
    for (unsigned long i = 0; i < numRows; i++)
        matrix[i] = prv_createRow(numCols, valueDefault);    
    
    return matrix;
}

//---------------------------------------------------------------

static struct prv_dataProbability_t *prv_createProbability(
                        unsigned long numRows, unsigned long numCols,
                        unsigned long row_current, unsigned long col_current,
                        double ***probabilityMatrix)
{
    struct prv_dataProbability_t *dataProbability;
    
    dataProbability = MALLOC(struct prv_dataProbability_t);
    
    dataProbability->numRows = numRows;
    dataProbability->numCols = numCols;
    
    dataProbability->row_current = row_current;  
    dataProbability->col_current = col_current;
    
    dataProbability->probabilityMatrix = ASSIGN_PP_NO_NULL(probabilityMatrix, double *);
    
    return dataProbability;
}

//---------------------------------------------------------------

static struct prv_dataProbability_t *prv_createProbabilityDefault(
                        unsigned long numRows, unsigned long numCols,
                        double valueDefault)
{
    unsigned long row_current, col_current;
    double **probabilityMatrix;
    
    probabilityMatrix = prv_createMatrix(
                            prv_NUM_ROW_MAX, prv_NUM_COL_MAX,
                            valueDefault);
    
    row_current = 0;
    col_current = 0;
    
    return prv_createProbability(
                        prv_NUM_ROW_MAX, prv_NUM_COL_MAX,
                        row_current, col_current, &probabilityMatrix);
}

//---------------------------------------------------------------

static void prv_destroyProbability(struct prv_dataProbability_t **dataProbability)
{
    assert_no_null(dataProbability);
    assert_no_null(*dataProbability);
    
    for (unsigned long i = 0; i < (*dataProbability)->numRows; i++)
        delete (*dataProbability)->probabilityMatrix[i];
    
    delete (*dataProbability)->probabilityMatrix;
    
    FREE_T(dataProbability, struct prv_dataProbability_t);
}

//---------------------------------------------------------------

static void i_nextPosition(
                    unsigned long numRows, unsigned long numCols,
                    unsigned long *row, unsigned long *col) 
{
    assert_no_null(row); 
    assert_no_null(col);
    
    *row = (*row + 1) % numRows; 
    *col = (*col + 1) % numCols;
}

//---------------------------------------------------------------

static bool prv_calculatePositionRandom(
                        struct prv_dataProbability_t *dataProbability,
                        unsigned long *row, unsigned long *col)
{
    unsigned long numAttempts;
    
    assert_no_null(dataProbability);
    assert_no_null(row);
    assert_no_null(col);
    
    numAttempts = 10;
    
    for (unsigned long i = 0; i < numAttempts; i++)
    {
        double numberRandom;
        unsigned long row_loc, col_loc;
        
        numberRandom = CMath::getRandomNumber();
        
        row_loc = dataProbability->row_current;
        col_loc = dataProbability->col_current;
        
        i_nextPosition(
                    dataProbability->numRows, dataProbability->numCols,
                    &dataProbability->row_current, &dataProbability->col_current);
                
        if (dataProbability->probabilityMatrix[row_loc][col_loc] < numberRandom)
        {
            *row = row_loc;
            *col = col_loc;
            
            return true;
        }
    }
    
    return false;
}
                        
//---------------------------------------------------------------

static struct prv_dataPrivate_t *prv_create(
                        struct prv_dataProbability_t **probabilityGrowing,
                        struct prv_dataProbability_t **probabilityStorm)
{
    struct prv_dataPrivate_t *dataPrivate;
    
    dataPrivate = MALLOC(struct prv_dataPrivate_t);
    
    dataPrivate->probabilityGrowing = ASSIGN_PP_NO_NULL(probabilityGrowing, struct prv_dataProbability_t);
    dataPrivate->probabilityStorm = ASSIGN_PP_NO_NULL(probabilityStorm, struct prv_dataProbability_t);
    
    return dataPrivate;    
} 

//---------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivate_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);
    
    prv_destroyProbability(&(*dataPrivate)->probabilityGrowing);
    prv_destroyProbability(&(*dataPrivate)->probabilityStorm);
    
    FREE_T(dataPrivate, struct prv_dataPrivate_t);
}

//---------------------------------------------------------------

CProbabilityTable::CProbabilityTable()
{
    struct prv_dataProbability_t *probabilityGrowing;
    struct prv_dataProbability_t *probabilityStorm;

    probabilityGrowing = prv_createProbabilityDefault(prv_NUM_ROW_MAX, prv_NUM_COL_MAX, prv_PROBABILITY_GROWNING_TREE_DEFAULT);
    probabilityStorm = prv_createProbabilityDefault(prv_NUM_ROW_MAX, prv_NUM_COL_MAX, prv_PROBABILITY_STORM_DEFAULT);
    
    m_dataPrivate = prv_create(&probabilityGrowing, &probabilityStorm);
}

//---------------------------------------------------------------

CProbabilityTable::~CProbabilityTable()
{
    prv_destroy(&m_dataPrivate);    
}

//---------------------------------------------------------------

bool CProbabilityTable::positionTreeGrowning(unsigned long *row, unsigned long *col)
{
    assert_no_null(m_dataPrivate);
    return prv_calculatePositionRandom(m_dataPrivate->probabilityGrowing, row, col);
}

//---------------------------------------------------------------

bool CProbabilityTable::positionStorm(unsigned long *row, unsigned long *col)
{
    assert_no_null(m_dataPrivate);
    return prv_calculatePositionRandom(m_dataPrivate->probabilityStorm, row, col);
}

//---------------------------------------------------------------

void CProbabilityTable::setEmptyTree(unsigned long row, unsigned long col)
{
    assert_no_null(m_dataPrivate);
    assert(row < m_dataPrivate->probabilityGrowing->numRows);
    assert(col < m_dataPrivate->probabilityGrowing->numCols);
    assert(fabs(m_dataPrivate->probabilityGrowing->probabilityMatrix[row][col] - 1.) < prv_TOLERANCE);
    
    m_dataPrivate->probabilityGrowing->probabilityMatrix[row][col] = prv_PROBABILITY_GROWNING_TREE_DEFAULT; 
}
