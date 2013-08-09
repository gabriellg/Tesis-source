/* -----------------------------------------------------------------------------
*  CGraphIO ( CGraph Input / Output file system )
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef CGRAPHIO_HPP_
#define CGRAPHIO_HPP_

typedef class IVertex* (*CGraphIO_FPtr_readVertex)(class IFileIn *in);
typedef void (*CGraphIO_FPtr_writeVertex)(const class IVertex *vertex, class IFileOut *out);
template<class> class CGraph;


template<class TVertex>
class CGraphIO
{
    public:
        static void print( const class CGraph<TVertex> *graph );

        static class CGraph<TVertex>* readGRF( class CFileInTxt *file );
        static class CGraph<TVertex>* readDOT( class CFileInTxt *file );
        static class CGraph<TVertex>* readGXL( class CFileInTxt *file );
        static class CGraph<TVertex>* readGXL( class CFileInTxt *file, const char* graphTagLabel, const char* nodeTagLabel );
        static class CGraph<TVertex>* readBin( class CFileInBin *file );

        static void writeGRF( const class CGraph<TVertex> *graph, class CFileOutTxt *file );
        static void writeDOT( const class CGraph<TVertex> *graph, class CFileOutTxt *file );
        static void writeGXL( const class CGraph<TVertex> *graph, class CFileOutTxt *file );
        static void writeGXL( const class CGraph<TVertex> *graph, class CFileOutTxt *file, const char* graphTagLabel, const char* nodeTagLabel );
        static void writeBin( const class CGraph<TVertex> *graph, class CFileOutBin *file );

//        static class CGraph<TVertex>* read( class IFileIn *in );
//        static void write( const class CGraph<TVertex> *graph, class IFileOut *out );
};

class PrvBaseGraphIO
{
    public:
        static void prv_print( const class CGraph<class IVertex> *graph );

        static class CGraph<class IVertex>* prv_readGRF( class CFileInTxt *file, CGraphIO_FPtr_readVertex func_readVertex );
        static class CGraph<class IVertex>* prv_readDOT( class CFileInTxt *file, CGraphIO_FPtr_readVertex func_readVertex );
        static class CGraph<class IVertex>* prv_readGXL( class CFileInTxt *file, CGraphIO_FPtr_readVertex func_readVertex, const char* graphTagLabel, const char* nodeTagLabel );
        static class CGraph<class IVertex>* prv_readBin( class CFileInBin *file, CGraphIO_FPtr_readVertex func_readVertex );

        static void prv_writeGRF( const class CGraph<class IVertex> *graph, class CFileOutTxt *file, CGraphIO_FPtr_writeVertex func_writeVertex );
        static void prv_writeDOT( const class CGraph<class IVertex> *graph, class CFileOutTxt *file, CGraphIO_FPtr_writeVertex func_writeVertex );
        static void prv_writeGXL( const class CGraph<class IVertex> *graph, class CFileOutTxt *file, CGraphIO_FPtr_writeVertex func_writeVertex, const char* graphTagLabel, const char* nodeTagLabel );
        static void prv_writeBin( const class CGraph<class IVertex> *graph, class CFileOutBin *file, CGraphIO_FPtr_writeVertex func_writeVertex );

        //static class CGraph<IVertex>* prv_read(CGraphIO_FPtr_readVertex func_readVertex, class IFileIn *in);
        //static void prv_write( const class CGraph<class IVertex> *graph, CGraphIO_FPtr_writeVertex func_writeVertex, class IFileOut *out);
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
template<class TVertex>
void CGraphIO<TVertex>::print( const class CGraph<TVertex> *graph )
{
    PrvBaseGraphIO::prv_print( (const class CGraph<class IVertex>*)graph );
}

//------------------------------------------------------------------------------
//
template<class TVertex>
class CGraph<TVertex>* CGraphIO<TVertex>::readGRF( class CFileInTxt *file )
{
    return (class CGraph<TVertex>*)PrvBaseGraphIO::prv_readGRF( file,
                                            (CGraphIO_FPtr_readVertex)TVertex::read );
}

//------------------------------------------------------------------------------
//
template<class TVertex>
class CGraph<TVertex>* CGraphIO<TVertex>::readDOT( class CFileInTxt *file )
{
    return (class CGraph<TVertex>*)PrvBaseGraphIO::prv_readDOT( file,
                                            (CGraphIO_FPtr_readVertex)TVertex::read );
}

//------------------------------------------------------------------------------
//
template<class TVertex>
class CGraph<TVertex>* CGraphIO<TVertex>::readGXL( class CFileInTxt *file )
{
    return (class CGraph<TVertex>*)PrvBaseGraphIO::prv_readGXL( file,
                                            (CGraphIO_FPtr_readVertex)TVertex::read,
                                            "label", "label" );
}

//------------------------------------------------------------------------------
//
template<class TVertex>
class CGraph<TVertex>* CGraphIO<TVertex>::readGXL( class CFileInTxt *file,
                                const char* graphTagLabel, const char* nodeTagLabel )
{
    return (class CGraph<TVertex>*)PrvBaseGraphIO::prv_readGXL( file,
                                            (CGraphIO_FPtr_readVertex)TVertex::read,
                                            graphTagLabel, nodeTagLabel );
}

//------------------------------------------------------------------------------
//
template<class TVertex>
class CGraph<TVertex>* CGraphIO<TVertex>::readBin( class CFileInBin *file )
{
    return (class CGraph<TVertex>*)PrvBaseGraphIO::prv_readBin( file,
                                            (CGraphIO_FPtr_readVertex)TVertex::read );
}

//------------------------------------------------------------------------------
//
template<class TVertex>
void CGraphIO<TVertex>::writeGRF( const class CGraph<TVertex> *graph, class CFileOutTxt *file )
{
    PrvBaseGraphIO::prv_writeGRF( (const class CGraph<class IVertex>*)graph,
                                  file,
                                  (CGraphIO_FPtr_writeVertex)TVertex::write );
}

//------------------------------------------------------------------------------
//
template<class TVertex>
void CGraphIO<TVertex>::writeDOT( const class CGraph<TVertex> *graph, class CFileOutTxt *file )
{
    PrvBaseGraphIO::prv_writeDOT( (const class CGraph<class IVertex>*)graph,
                                  file,
                                  (CGraphIO_FPtr_writeVertex)TVertex::write );
}

//------------------------------------------------------------------------------
//
template<class TVertex>
void CGraphIO<TVertex>::writeGXL( const class CGraph<TVertex> *graph, class CFileOutTxt *file )
{
    PrvBaseGraphIO::prv_writeGXL( (const class CGraph<class IVertex>*)graph,
                                  file,
                                  (CGraphIO_FPtr_writeVertex)TVertex::write,
                                  "label", "label" );
}

//------------------------------------------------------------------------------
//
template<class TVertex>
void CGraphIO<TVertex>::writeGXL( const class CGraph<TVertex> *graph, class CFileOutTxt *file,
                                  const char* graphTagLabel, const char* nodeTagLabel )
{
    PrvBaseGraphIO::prv_writeGXL( (const class CGraph<class IVertex>*)graph,
                                  file,
                                  (CGraphIO_FPtr_writeVertex)TVertex::write,
                                  graphTagLabel, nodeTagLabel );
}

//------------------------------------------------------------------------------
//
template<class TVertex>
void CGraphIO<TVertex>::writeBin( const class CGraph<TVertex> *graph, class CFileOutBin *file )
{
    PrvBaseGraphIO::prv_writeBin( (const class CGraph<class IVertex>*)graph,
                                    file,
                                    (CGraphIO_FPtr_writeVertex)TVertex::write );
}

/*
//------------------------------------------------------------------------------
//
template<class TVertex>
class CGraph<TVertex>* CGraphIO<TVertex>::read(class IFileIn *in)
{
    return (class CGraph<TVertex>*)PrvBaseGraphIO::prv_read( (CGraphIO_FPtr_readVertex)TVertex::read, in );
}

//------------------------------------------------------------------------------
//
template<class TVertex>
void CGraphIO<TVertex>::write(const class CGraph<TVertex> *graph, class IFileOut *out)
{
    PrvBaseGraphIO::prv_write( (const class CGraph<class IVertex>*)graph,
                               (CGraphIO_FPtr_writeVertex)TVertex::write,
                               out );
}
*/
#endif
