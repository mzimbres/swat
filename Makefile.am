
ACLOCAL_AMFLAGS = -I m4

AM_COLOR_TESTS  = always

AM_LDFLAGS  = -L${ROOTLIBDIR} ${ROOTLIBS} ${ROOTAUXLIBS} -lHistPainter

include_HEADERS =
lib_LTLIBRARIES =
CLEANFILES      =

#________________________________________________________________

FFTPACKH =  $(top_srcdir)/bluestein.h   \
            $(top_srcdir)/ls_fft.h      \
	    $(top_srcdir)/fftpack.h

FFTPACKS =  $(top_srcdir)/bluestein.c   \
            $(top_srcdir)/ls_fft.c      \
	    $(top_srcdir)/fftpack.c

include_HEADERS +=  $(FFTPACKH)

lib_LTLIBRARIES += libfftpack.la

libfftpack_la_CPPFLAGS = -I$(top_srcdir)


libfftpack_la_SOURCES = $(FFTPACKS)

#______________________________________________________

HEALPIXH = $(top_srcdir)/alm_healpix_tools.h  \
           $(top_srcdir)/alm_map_tools.h      \
           $(top_srcdir)/fftpack_support.h    \
           $(top_srcdir)/lsconstants.h        \
           $(top_srcdir)/message_error.h      \
           $(top_srcdir)/cxxutils.h           \
           $(top_srcdir)/vec3.h               \
           $(top_srcdir)/ylmgen.h             \
           $(top_srcdir)/geom_utils.h         \
           $(top_srcdir)/pointing.h           \
           $(top_srcdir)/Alm.h                \
           $(top_srcdir)/xcomplex.h           \
           $(top_srcdir)/arr.h                \
           $(top_srcdir)/Healpix_Base.h       \
           $(top_srcdir)/Healpix_Map.h


HEALPIXS = $(top_srcdir)/alm_healpix_tools.cc \
           $(top_srcdir)/alm_map_tools.cc     \
           $(top_srcdir)/healpix_base.cc

include_HEADERS += $(HEALPIXH)

lib_LTLIBRARIES += libhealpix.la

libhealpix_la_CPPFLAGS = -I$(top_srcdir)

libhealpix_la_SOURCES = $(HEALPIXS)

#________________________________________________________________

# headers for which dictionaries must be generated.
SWATDH = $(top_srcdir)/TWignerd.h              \
         $(top_srcdir)/TSmallWignerd.h         \
	 $(top_srcdir)/TSlm.h                  \
	 $(top_srcdir)/TSFunc.h                \
	 $(top_srcdir)/TDKernel.h              \
	 $(top_srcdir)/TAlm.h                  \
	 $(top_srcdir)/TCoeffInfo.h            \
	 $(top_srcdir)/TSwatF.h                \
	 $(top_srcdir)/TSwatB.h                \
	 $(top_srcdir)/TVMap.h                 \
	 $(top_srcdir)/TSkyMap.h               \
	 $(top_srcdir)/THealpixMap.h           \
	 $(top_srcdir)/TWavMap.h               \
	 $(top_srcdir)/TSphHarmB.h             \
	 $(top_srcdir)/TSphHarmF.h             \
	 $(top_srcdir)/TWavelet.h              \
	 $(top_srcdir)/TShift.h                \
	 $(top_srcdir)/TEulerAngle.h           \
	 $(top_srcdir)/TAngDistance.h          \
	 $(top_srcdir)/TCorrSelector.h         \
	 $(top_srcdir)/TCorrSelectorCRPropa.h  \
	 $(top_srcdir)/TCounter.h              \
	 $(top_srcdir)/TCounterCRPropa.h       \
	 $(top_srcdir)/TMapSelector.h          \
	 $(top_srcdir)/TMapSelectorCRPropa.h   \
	 $(top_srcdir)/TStripeAnalysis.h       \
	 $(top_srcdir)/TSourcesFinder.h        \
	 $(top_srcdir)/TAnalysis.h             \
	 $(top_srcdir)/TAuxFunc.h

SWATS = $(top_srcdir)/TWignerd.cxx               \
        $(top_srcdir)/TSmallWignerd.cxx          \
	$(top_srcdir)/TSlm.cxx                   \
	$(top_srcdir)/TSFunc.cxx                 \
	$(top_srcdir)/TDKernel.cxx               \
	$(top_srcdir)/TAlm.cxx                   \
	$(top_srcdir)/TCoeffInfo.cxx             \
	$(top_srcdir)/TSwatF.cxx                 \
	$(top_srcdir)/TVMap.cxx                  \
	$(top_srcdir)/TSkyMap.cxx                \
	$(top_srcdir)/THealpixMap.cxx            \
	$(top_srcdir)/TSwatB.cxx                 \
	$(top_srcdir)/TWavMap.cxx                \
	$(top_srcdir)/TEulerAngle.cxx            \
	$(top_srcdir)/TSphHarmB.cxx              \
	$(top_srcdir)/TSphHarmF.cxx              \
	$(top_srcdir)/TWavelet.cxx               \
	$(top_srcdir)/TShift.cxx                 \
	$(top_srcdir)/TAngDistance.cxx           \
	$(top_srcdir)/TCorrSelector.cxx          \
	$(top_srcdir)/TCorrSelectorCRPropa.cxx   \
	$(top_srcdir)/TCounter.cxx               \
	$(top_srcdir)/TCounterCRPropa.cxx        \
	$(top_srcdir)/TMapSelector.cxx           \
	$(top_srcdir)/TMapSelectorCRPropa.cxx    \
	$(top_srcdir)/TSourcesFinder.cxx         \
	$(top_srcdir)/TAnalysis.cxx              \
	$(top_srcdir)/TAuxFunc.cxx

## Generates dictionary
dictionary.cxx: $(SWATDH)
	@echo "Generating dictionary $@..."
	$(ROOTCINT) -f $@ -c $^

include_HEADERS += $(SWATDH) swat.h

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

bin_PROGRAMS      = swat swatsim swatfind

swat_SOURCES  = $(top_srcdir)/swat.cxx
swatsim_SOURCES  = $(top_srcdir)/swatsim.cxx
swatfind_SOURCES  = $(top_srcdir)/swatfind.cxx

#________________________________________________________________

check_PROGRAMS    = alm2map2alm 

alm2map2alm_SOURCES  = $(top_srcdir)/tests/alm2map2alm.cxx

TESTS =  alm2map2alm swat 

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
                    $(MACRODIR)/read_rafael.C             \
	            $(MACRODIR)/wig_graph.C
#________________________________________________________________

EXTRA_DIST = $(CONFIGDIR)/load.C.in                  \
             $(top_srcdir)/tests/map2alm.C           \
             $(top_srcdir)/pgfplots/skymap.tar.gz    \
             $(top_srcdir)/pgfplots/wignerd.tar.gz   \
             $(top_srcdir)/pgfplots/corr_graph.tex   \
             $(top_srcdir)/pgfplots/multiplicity.tex \
             $(top_srcdir)/doc/skymap.pdf            \
             $(top_srcdir)/doc/skymap-sim.pdf        \
             $(top_srcdir)/doc/wignerpolar.pdf       \
             $(top_srcdir)/doc/wigner.pdf            \
             $(top_srcdir)/doc/corr_graph-sim.pdf    \
             $(top_srcdir)/doc/multiplicity-sim.pdf  \
             $(top_srcdir)/doc/swatguide.tex


