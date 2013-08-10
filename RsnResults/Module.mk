# module for RsnResults
# Author: Jan Musinsky
# Date:   2013-08-10

MODULE	= RsnResults
MODDIR	= RsnResults
FILES	= TRsnResults TGraphRsnErrors

LNKDEF	= $(MODDIR)/$(LNKFILE).$(HdrSuf)
SRCS	= $(patsubst %,$(MODDIR)/%.$(SrcSuf),$(FILES))
HDRS	= $(SRCS:.$(SrcSuf)=.$(HdrSuf))
DICT	= $(TMPDIR)/$(MODDIR)/$(DICTPREFIX)$(MODULE).$(SrcSuf)
DICTH	= $(DICT:.$(SrcSuf)=.$(HdrSuf))
DICTO	= $(DICT:.$(SrcSuf)=.$(ObjSuf))
OBJS	= $(patsubst %.$(SrcSuf),$(TMPDIR)/%.$(ObjSuf),$(SRCS))
MODLIB	= $(LIBDIR)/$(LIBPREFIX)$(MODULE).$(DllSuf)

RSNRESULTS := $(OBJS) $(DICT) $(DICTH) $(DICTO)
ifeq (distclean,$(findstring distclean,$(MAKECMDGOALS)))
RSNRESULTS += $(MODLIB) $(subst $(MODDIR)/,$(INCDIR)/,$(HDRS)) # '/' important
endif

# used in the main Makefile
ALLLIBS	+= $(MODLIB)
ALLHDRS	+= $(HDRS)
ALLDIST	+= $(SRCS) $(HDRS) $(LNKDEF) $(MODDIR)/$(MODMAKEFILE)
ALLDEPEND += $(SRCS)

$(MODDIR):	$(MODLIB)

$(MODLIB):	$(OBJS) $(DICTO)

$(DICT):	$(HDRS) $(LNKDEF)

clean-$(MODDIR):
		@rm -fv $(RSNRESULTS)

clean:		clean-$(MODDIR)

distclean-$(MODDIR):
		@rm -fv $(RSNRESULTS)

distclean:	distclean-$(MODDIR)
