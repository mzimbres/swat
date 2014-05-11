
ACLOCAL_AMFLAGS = -I m4

AM_COLOR_TESTS = always

AM_LDFLAGS = -L${ROOTLIBDIR} ${ROOTLIBS} ${ROOTAUXLIBS} -lHistPainter $(FFTW_LIBS)
AM_CPPFLAGS = -I${ROOTINCDIR} ${ROOTCFLAGS} ${ROOTAUXCLAGS} -I$(top_srcdir)


# Add the line below to AM_LDFLAGS for Boost programm options.
# $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB)

lib_LTLIBRARIES =
CLEANFILES =
EXTRA_DIST =
swat_inc = $(top_srcdir)/swat
swat_src = $(top_srcdir)/libs
healpix_inc = $(top_srcdir)/swat/healpix
healpix_src = $(top_srcdir)/libs/healpix

#________________________________________________________________

FFTPACKH = $(healpix_inc)/bluestein.h \
           $(healpix_inc)/ls_fft.h \
	   $(healpix_inc)/fftpack.h

FFTPACKS = $(healpix_src)/bluestein.c \
           $(healpix_src)/ls_fft.c \
	   $(healpix_src)/fftpack.c

lib_LTLIBRARIES += libfftpack.la

libfftpack_la_SOURCES = $(FFTPACKS)

EXTRA_DIST += $(FFTPACKH)

#______________________________________________________

HEALPIXH = $(healpix_inc)/alm_healpix_tools.h  \
           $(healpix_inc)/alm_map_tools.h      \
           $(healpix_inc)/fftpack_support.h    \
           $(healpix_inc)/lsconstants.h        \
           $(healpix_inc)/message_error.h      \
           $(healpix_inc)/cxxutils.h           \
           $(healpix_inc)/vec3.h               \
           $(healpix_inc)/ylmgen.h             \
           $(healpix_inc)/geom_utils.h         \
           $(healpix_inc)/pointing.h           \
           $(healpix_inc)/Alm.h                \
           $(healpix_inc)/xcomplex.h           \
           $(healpix_inc)/arr.h                \
           $(healpix_inc)/Healpix_Base.h       \
           $(healpix_inc)/Healpix_Map.h

EXTRA_DIST += $(HEALPIXH)

HEALPIXS = $(healpix_src)/alm_healpix_tools.cc \
           $(healpix_src)/alm_map_tools.cc \
           $(healpix_src)/healpix_base.cc

lib_LTLIBRARIES += libhealpix.la

libhealpix_la_SOURCES = $(HEALPIXS)

#________________________________________________________________

# headers for which no dictionaries are needed
SWATH = $(swat_inc)/TFFTWAlloc.h \
        $(swat_inc)/swat.h \
        $(swat_inc)/to_number.h

# headers for which dictionaries must be generated.
SWATDH = $(swat_inc)/TWignerd.h              \
         $(swat_inc)/TSmallWignerd.h         \
	 $(swat_inc)/TSlm.h                  \
	 $(swat_inc)/TSFunc.h                \
	 $(swat_inc)/TDKernel.h              \
	 $(swat_inc)/TAlm.h                  \
	 $(swat_inc)/TCoeffInfo.h            \
	 $(swat_inc)/TSwatF.h                \
	 $(swat_inc)/TSwatB.h                \
	 $(swat_inc)/TVMap.h                 \
	 $(swat_inc)/TSkyMap.h               \
	 $(swat_inc)/THealpixMap.h           \
	 $(swat_inc)/TWavMap.h               \
	 $(swat_inc)/TSphHarmB.h             \
	 $(swat_inc)/TSphHarmF.h             \
	 $(swat_inc)/TWavelet.h              \
	 $(swat_inc)/TShift.h                \
	 $(swat_inc)/TEulerAngle.h           \
	 $(swat_inc)/TAngDistance.h          \
	 $(swat_inc)/TCorrSelector.h         \
	 $(swat_inc)/TCorrSelectorCRPropa.h  \
	 $(swat_inc)/TCounter.h              \
	 $(swat_inc)/TCounterCRPropa.h       \
	 $(swat_inc)/TMapSelector.h          \
	 $(swat_inc)/TMapSelectorCRPropa.h   \
	 $(swat_inc)/TStripeAnalysis.h       \
	 $(swat_inc)/TSourcesFinder.h        \
	 $(swat_inc)/TAnalysis.h             \
	 $(swat_inc)/TAuxFunc.h

EXTRA_DIST += $(SWATH) $(SWATDH)

SWATS = $(swat_src)/TWignerd.cxx               \
        $(swat_src)/TSmallWignerd.cxx          \
	$(swat_src)/TSlm.cxx                   \
	$(swat_src)/TSFunc.cxx                 \
	$(swat_src)/TDKernel.cxx               \
	$(swat_src)/TAlm.cxx                   \
	$(swat_src)/TCoeffInfo.cxx             \
	$(swat_src)/TSwatF.cxx                 \
	$(swat_src)/TVMap.cxx                  \
	$(swat_src)/TSkyMap.cxx                \
	$(swat_src)/THealpixMap.cxx            \
	$(swat_src)/TSwatB.cxx                 \
	$(swat_src)/TWavMap.cxx                \
	$(swat_src)/TEulerAngle.cxx            \
	$(swat_src)/TSphHarmB.cxx              \
	$(swat_src)/TSphHarmF.cxx              \
	$(swat_src)/TWavelet.cxx               \
	$(swat_src)/TShift.cxx                 \
	$(swat_src)/TAngDistance.cxx           \
	$(swat_src)/TCorrSelector.cxx          \
	$(swat_src)/TCorrSelectorCRPropa.cxx   \
	$(swat_src)/TCounter.cxx               \
	$(swat_src)/TCounterCRPropa.cxx        \
	$(swat_src)/TMapSelector.cxx           \
	$(swat_src)/TMapSelectorCRPropa.cxx    \
	$(swat_src)/TSourcesFinder.cxx         \
	$(swat_src)/TAnalysis.cxx              \
	$(swat_src)/TAuxFunc.cxx

## Generates dictionary
dictionary.cxx: $(SWATDH)
	@echo "Generating dictionary $@..."
	$(ROOTCINT) -f $@ -c $^

lib_LTLIBRARIES += libswat.la

libswat_la_SOURCES = $(SWATS)

nodist_libswat_la_SOURCES = dictionary.cxx

libswat_la_LDFLAGS = -L${ROOTLIBDIR} ${ROOTLIBS} ${ROOTAUXLIBS} \
                     -lHistPainter -lhealpix -lfftpack $(FFTW_LIBS)

CLEANFILES += dictionary.cxx dictionary.h

#________________________________________________________________

LDADD  = libswat.la libhealpix.la libfftpack.la

# Add the line below to AM_CPPFLAGS for boost programm options
# $(BOOST_CPPFLAGS)

#________________________________________________________________

bin_PROGRAMS = swat swat_sim \
               swat_find swat_alm \
	       swat_gen swat_prob \
	       swat_kernel \
	       swat_wav_dump \
	       swat_sph_dump \
	       swat_coverage \
	       swat_sf_dump

swat_SOURCES           = $(swat_src)/swat.cxx
swat_sim_SOURCES       = $(swat_src)/swat_sim.cxx
swat_find_SOURCES      = $(swat_src)/swat_find.cxx
swat_alm_SOURCES       = $(swat_src)/swat_alm.cxx
swat_gen_SOURCES       = $(swat_src)/swat_gen.cxx
swat_prob_SOURCES      = $(swat_src)/swat_prob.cxx
swat_kernel_SOURCES    = $(swat_src)/swat_kernel.cxx
swat_wav_dump_SOURCES  = $(swat_src)/swat_wav_dump.cxx
swat_sph_dump_SOURCES  = $(swat_src)/swat_sph_dump.cxx
swat_sf_dump_SOURCES   = $(swat_src)/swat_sf_dump.cxx
swat_coverage_SOURCES  = $(swat_src)/swat_coverage.cxx

#________________________________________________________________

check_PROGRAMS = alm2map2alm 

alm2map2alm_SOURCES = $(top_srcdir)/tests/alm2map2alm.cxx

TESTS = alm2map2alm swat 

#________________________________________________________________


do_subst = sed -e 's,libdir,$(libdir),g'             \
               -e 's,pkgdatadir,$(pkgdatadir),g'     \
               -e 's,incdir,$(includedir),g'


CONFIGDIR = $(top_srcdir)/config

load.C: $(CONFIGDIR)/load.C.in
	$(do_subst) < $^ > $@

pkgdata_DATA = load.C


CLEANFILES += load.C

#________________________________________________________________

MACRODIR = $(top_srcdir)/macros

dist_pkgdata_DATA = $(MACRODIR)/DrawLatitudesLongitudes.C \
                    $(MACRODIR)/benchmark_fft_gsl.C       \
		    $(MACRODIR)/convert_auger.C           \
	            $(MACRODIR)/convert_herald.C          \
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

EXTRA_DIST += $(CONFIGDIR)/load.C.in                       \
              $(top_srcdir)/tests/map2alm.C                \
              $(top_srcdir)/pgfplots/skymap.tar.gz         \
              $(top_srcdir)/pgfplots/wignerd.tar.gz        \
              $(top_srcdir)/pgfplots/corr_graph.tex        \
              $(top_srcdir)/pgfplots/cprob.tex             \
              $(top_srcdir)/pgfplots/wprob.tex             \
              $(top_srcdir)/pgfplots/kernel.tex            \
              $(top_srcdir)/pgfplots/multiplicity.tex      \
              $(top_srcdir)/doc/skymap.pdf                 \
              $(top_srcdir)/doc/skymap-sim.pdf             \
              $(top_srcdir)/doc/mult-cand.pdf              \
              $(top_srcdir)/doc/crpropa.pdf                \
              $(top_srcdir)/doc/wignerpolar.pdf            \
              $(top_srcdir)/doc/wigner.pdf                 \
              $(top_srcdir)/doc/corr_graph-sim.pdf         \
              $(top_srcdir)/doc/multiplicity-sim.pdf       \
              $(top_srcdir)/doc/swatguide.tex              \
              $(top_srcdir)/doc/healpixGridRefinement.jpg  \
              $(top_srcdir)/doc/cylindrical-f1.pdf         \
              $(top_srcdir)/doc/cylindrical-f2.pdf         \
              $(top_srcdir)/doc/cylindrical-f3.pdf         \
              $(top_srcdir)/fig/sf-f0.pdf

