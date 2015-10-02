# module for RsnResults
# Author: Jan Musinsky
# Date:   2015-10-02

MODULE	= RsnResults
MODDIR	= RsnResults
FILES	= TRsnResults TRsnFragment TRsnGroup TGraphRsnErrors TRsnSparseHandler

RSNRESULTSDOPT	:= $(LIBPREFIX)$(MODULE)
RSNRESULTSCLEAN	:= $(OBJS) $(DICT) $(DICTH) $(DICTO)
ifneq (,$(findstring distclean,$(MAKECMDGOALS)))
RSNRESULTSCLEAN	+= $(MODLIB) $(subst $(MODDIR)/,$(INCDIR)/,$(HDRS)) # '/' important
RSNRESULTSCLEAN	+= $(subst $(DllSuf),$(MAPSUF),$(MODLIB))
RSNRESULTSCLEAN	+= $(subst .$(DllSuf),*.$(PCMSUF),$(MODLIB))
endif

# used in the main Makefile
ALLLIBS		+= $(MODLIB)
ALLHDRS		+= $(HDRS)
ALLDIST		+= $(SRCS) $(HDRS) $(LNKDEF) $(MODDIR)/$(MODMAKEFILE)
ALLDEPEND	+= $(SRCS)

$(DICT)DictOpt	= $(call DictOpt,$(RSNRESULTSDOPT))
#$(MODLIB)Extra	= # nothing yet

# local rules
$(MODDIR):	$(MODLIB)

$(MODLIB):	$(OBJS) $(DICTO)

$(DICT):	$(HDRS) $(LNKDEF)

clean-$(MODDIR):
		@rm -fv $(RSNRESULTSCLEAN)

clean:		clean-$(MODDIR)

distclean-$(MODDIR):
		@rm -fv $(RSNRESULTSCLEAN)

distclean:	distclean-$(MODDIR)
