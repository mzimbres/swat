
ACLOCAL_AMFLAGS = -I m4

AM_COLOR_TESTS  = always

AM_LDFLAGS  = -L${ROOTLIBDIR} ${ROOTLIBS} ${ROOTAUXLIBS} -lHistPainter

include_HEADERS =
lib_LTLIBRARIES =
CLEANFILES      =

#________________________________________________________________

FFTPACKDIR  = $(top_srcdir)/fftpack
FFTPACKDIRI = $(FFTPACKDIR)/inc
FFTPACKDIRS = $(FFTPACKDIR)/src

FFTPACKH =  $(FFTPACKDIRI)/bluestein.h   \
            $(FFTPACKDIRI)/ls_fft.h      \
	    $(FFTPACKDIRI)/fftpack.h

FFTPACKS =  $(FFTPACKDIRS)/bluestein.c   \
            $(FFTPACKDIRS)/ls_fft.c      \
	    $(FFTPACKDIRS)/fftpack.c

include_HEADERS +=  $(FFTPACKH)

lib_LTLIBRARIES += libfftpack.la

libfftpack_la_CPPFLAGS = -I$(FFTPACKDIRI)


libfftpack_la_SOURCES = $(FFTPACKS)

#______________________________________________________

HEALPIXDIR  = $(top_srcdir)/healpix
HEALPIXDIRI = $(HEALPIXDIR)/inc
HEALPIXDIRS = $(HEALPIXDIR)/src

HEALPIXH = $(HEALPIXDIRI)/alm_healpix_tools.h  \
           $(HEALPIXDIRI)/alm_map_tools.h      \
           $(HEALPIXDIRI)/fftpack_support.h    \
           $(HEALPIXDIRI)/lsconstants.h        \
           $(HEALPIXDIRI)/message_error.h      \
           $(HEALPIXDIRI)/cxxutils.h           \
           $(HEALPIXDIRI)/vec3.h               \
           $(HEALPIXDIRI)/ylmgen.h             \
           $(HEALPIXDIRI)/geom_utils.h         \
           $(HEALPIXDIRI)/pointing.h           \
           $(HEALPIXDIRI)/Alm.h                \
           $(HEALPIXDIRI)/xcomplex.h           \
           $(HEALPIXDIRI)/arr.h                \
           $(HEALPIXDIRI)/Healpix_Base.h       \
           $(HEALPIXDIRI)/Healpix_Map.h


HEALPIXS = $(HEALPIXDIRS)/alm_healpix_tools.cc \
           $(HEALPIXDIRS)/alm_map_tools.cc     \
           $(HEALPIXDIRS)/healpix_base.cc

include_HEADERS += $(HEALPIXH)

lib_LTLIBRARIES += libhealpix.la

libhealpix_la_CPPFLAGS = -I$(HEALPIXDIRI) -I$(FFTPACKDIRI)

libhealpix_la_SOURCES = $(HEALPIXS)

#________________________________________________________________

SWATDIR  = $(top_srcdir)/swat
SWATDIRI = $(SWATDIR)/inc
SWATDIRS = $(SWATDIR)/src

SWATNDH = $(SWATDIRI)/swat.h

# headers for which dictionaries must be generated.
SWATDH = $(SWATDIRI)/TWignerd.h              \
         $(SWATDIRI)/TSmallWignerd.h         \
	 $(SWATDIRI)/TSlm.h                  \
	 $(SWATDIRI)/TSFunc.h                \
	 $(SWATDIRI)/TDKernel.h              \
	 $(SWATDIRI)/TAlm.h                  \
	 $(SWATDIRI)/TCoeffInfo.h            \
	 $(SWATDIRI)/TSwatF.h                \
	 $(SWATDIRI)/TSwatB.h                \
	 $(SWATDIRI)/TVMap.h                 \
	 $(SWATDIRI)/TSkyMap.h               \
	 $(SWATDIRI)/THealpixMap.h           \
	 $(SWATDIRI)/TWavMap.h               \
	 $(SWATDIRI)/TSphHarmB.h             \
	 $(SWATDIRI)/TSphHarmF.h             \
	 $(SWATDIRI)/TWavelet.h              \
         $(SWATDIRI)/TParser.h               \
	 $(SWATDIRI)/TShift.h                \
	 $(SWATDIRI)/TEulerAngle.h           \
	 $(SWATDIRI)/TAngDistance.h          \
	 $(SWATDIRI)/TCorrSelector.h         \
	 $(SWATDIRI)/TCorrSelectorCRPropa.h  \
	 $(SWATDIRI)/TCounter.h              \
	 $(SWATDIRI)/TCounterCRPropa.h       \
	 $(SWATDIRI)/TMapSelector.h          \
	 $(SWATDIRI)/TMapSelectorCRPropa.h   \
	 $(SWATDIRI)/TStripeAnalysis.h       \
	 $(SWATDIRI)/TSourcesFinder.h        \
	 $(SWATDIRI)/TAnalysis.h             \
	 $(SWATDIRI)/TAuxFunc.h

SWATS = $(SWATDIRS)/TWignerd.cxx               \
        $(SWATDIRS)/TSmallWignerd.cxx          \
	$(SWATDIRS)/TSlm.cxx                   \
	$(SWATDIRS)/TSFunc.cxx                 \
	$(SWATDIRS)/TDKernel.cxx               \
	$(SWATDIRS)/TAlm.cxx                   \
	$(SWATDIRS)/TCoeffInfo.cxx             \
	$(SWATDIRS)/TSwatF.cxx                 \
	$(SWATDIRS)/TVMap.cxx                  \
	$(SWATDIRS)/TSkyMap.cxx                \
	$(SWATDIRS)/THealpixMap.cxx            \
	$(SWATDIRS)/TSwatB.cxx                 \
	$(SWATDIRS)/TWavMap.cxx                \
	$(SWATDIRS)/TEulerAngle.cxx            \
	$(SWATDIRS)/TSphHarmB.cxx              \
	$(SWATDIRS)/TSphHarmF.cxx              \
	$(SWATDIRS)/TWavelet.cxx               \
        $(SWATDIRS)/TParser.cxx                \
	$(SWATDIRS)/TShift.cxx                 \
	$(SWATDIRS)/TAngDistance.cxx           \
	$(SWATDIRS)/TCorrSelector.cxx          \
	$(SWATDIRS)/TCorrSelectorCRPropa.cxx   \
	$(SWATDIRS)/TCounter.cxx               \
	$(SWATDIRS)/TCounterCRPropa.cxx        \
	$(SWATDIRS)/TMapSelector.cxx           \
	$(SWATDIRS)/TMapSelectorCRPropa.cxx    \
	$(SWATDIRS)/TSourcesFinder.cxx         \
	$(SWATDIRS)/TAnalysis.cxx              \
	$(SWATDIRS)/TAuxFunc.cxx

#SWATS = $(SWATDH:.h=.cxx)

## Generates dictionary
dictionary.cxx: $(SWATDH)
	@echo "Generating dictionary $@..."
	$(ROOTCINT) -f $@ -c $^

include_HEADERS += $(SWATDH) $(SWATNDH) 

lib_LTLIBRARIES += libswat.la

libswat_la_SOURCES        = $(SWATS)

nodist_libswat_la_SOURCES = dictionary.cxx

libswat_la_CPPFLAGS      = -I${ROOTINCDIR} ${ROOTCFLAGS} ${ROOTAUXCLAGS} \
                           -I$(SWATDIRI) -I$(HEALPIXDIRI)

libswat_la_LDFLAGS       = -L${ROOTLIBDIR} ${ROOTLIBS} ${ROOTAUXLIBS} \
                           -lhealpix -lfftpack

CLEANFILES += dictionary.cxx dictionary.h

#________________________________________________________________

LDADD  = libswat.la libhealpix.la libfftpack.la

AM_CPPFLAGS = -I${ROOTINCDIR} ${ROOTCFLAGS} ${ROOTAUXCLAGS} -I$(SWATDIRI)

#________________________________________________________________

bin_PROGRAMS      = smfilter

smfilter_SOURCES  = $(top_srcdir)/utils/src/smfilter.cxx

#________________________________________________________________

check_PROGRAMS    = alm2map2alm parser

alm2map2alm_SOURCES  = $(top_srcdir)/tests/alm2map2alm.cxx

parser_SOURCES  = $(top_srcdir)/tests/parser.cxx

TESTS =  parser alm2map2alm smfilter 

#________________________________________________________________


do_subst = sed -e 's,libdir,$(libdir),g'             \
               -e 's,pkgdatadir,$(pkgdatadir),g'     \
               -e 's,incdir,$(includedir),g'


CONFIGDIR = $(top_srcdir)/config

load.C: $(CONFIGDIR)/load.C.in
	$(do_subst) < $^ > $@

pkgdata_DATA = load.C


CLEANFILES        += load.C

#________________________________________________________________

MACRODIR = $(top_srcdir)/macros

dist_pkgdata_DATA = $(MACRODIR)/DrawLatitudesLongitudes.C \
                    $(MACRODIR)/benchmark_fft_gsl.C       \
		    $(MACRODIR)/convert_auger.C           \
	            $(MACRODIR)/convert_herald.C          \
	            $(MACRODIR)/kernel.C                  \
                    $(MACRODIR)/makehtml.C                \
                    $(MACRODIR)/mystyle.C                 \
	            $(MACRODIR)/scale_func.C              \
                    $(MACRODIR)/sph_harm.C                \
                    $(MACRODIR)/wav_view.C                \
                    $(MACRODIR)/show_angles.C             \
                    $(MACRODIR)/graph_view.C              \
                    $(MACRODIR)/swat.C                    \
	            $(MACRODIR)/wig_graph.C
#________________________________________________________________

EXTRA_DIST = $(CONFIGDIR)/load.C.in        \
             $(top_srcdir)/tests/map2alm.C    \
             $(top_srcdir)/doc/swatguide.pdf


