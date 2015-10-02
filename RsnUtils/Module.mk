# module for RsnUtils
# Author: Jan Musinsky
# Date:   2015-10-02

MODULE	= RsnUtils
MODDIR	= RsnUtils
FILES	= TRsnUtils

RSNUTILSDOPT	:= $(LIBPREFIX)$(MODULE)
RSNUTILSCLEAN	:= $(OBJS) $(DICT) $(DICTH) $(DICTO)
ifneq (,$(findstring distclean,$(MAKECMDGOALS)))
RSNUTILSCLEAN	+= $(MODLIB) $(subst $(MODDIR)/,$(INCDIR)/,$(HDRS)) # '/' important
RSNUTILSCLEAN	+= $(subst $(DllSuf),$(MAPSUF),$(MODLIB))
RSNUTILSCLEAN	+= $(subst .$(DllSuf),*.$(PCMSUF),$(MODLIB))
endif

# used in the main Makefile
ALLLIBS		+= $(MODLIB)
ALLHDRS		+= $(HDRS)
ALLDIST		+= $(SRCS) $(HDRS) $(LNKDEF) $(MODDIR)/$(MODMAKEFILE)
ALLDEPEND	+= $(SRCS)

$(DICT)DictOpt	= $(call DictOpt,$(RSNUTILSDOPT))
#$(MODLIB)Extra	= # nothing yet

# local rules
$(MODDIR):	$(MODLIB)

$(MODLIB):	$(OBJS) $(DICTO)

$(DICT):	$(HDRS) $(LNKDEF)

clean-$(MODDIR):
		@rm -fv $(RSNUTILSCLEAN)

clean:		clean-$(MODDIR)

distclean-$(MODDIR):
		@rm -fv $(RSNUTILSCLEAN)

distclean:	distclean-$(MODDIR)
