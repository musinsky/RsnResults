# Top level Makefile for RsnResults
# Author: Jan Musinsky
# Date:   2013-08-10

include $(ROOTSYS)/etc/Makefile.arch
HdrSuf		= h
CXXFLAGS	+= -Wextra
ifeq ($(MAKECMDGOALS),debug)
CXXFLAGS	+= -DDEBUG
endif

MODULES		= RsnResults

MODMAKEFILE	= Module.mk
LNKFILE		= LinkDef
DICTPREFIX	= G__
INCDIR		= include
TMPDIR		= tmp
LIBDIR		= lib
LIBPREFIX	= lib
DISTSRCNAME	= RsnResults_$(shell date +%F).source
MAKEDEPEND	= rmkdepend
DEPENDFILE	= $(TMPDIR)/Make-depend
NODEPEND        = clean distclean distsrc

# all
ALLHDRS		:=
ALLLIBS		:=
ALLDIST		:= Makefile
ALLDEPEND	:=

# verbatim variables
define checkdir
@if [ ! -d $@ ]; then mkdir -p $(dir $@); fi
endef

define emptyfile
$(checkdir)
@if [ ! -e $@ ]; then > $@; fi
endef

# rules
.SUFFIXES: # delete the default suffixes
.PRECIOUS: $(INCDIR)/%.$(HdrSuf) # preserve intermediate files

$(TMPDIR)/%.$(ObjSuf) : %.$(SrcSuf)
			$(checkdir)
			@$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@
			@echo -e "$@ done"

$(DICTPREFIX)%.$(SrcSuf) :
			@echo "Generating dictionary $@ ..."
			$(checkdir)
			@rootcint -f $@ -c -I$(INCDIR) $^

$(DICTPREFIX)%.$(ObjSuf) : $(DICTPREFIX)%.$(SrcSuf)
			$(checkdir)
			@$(CXX) $(CXXFLAGS) -I$(INCDIR) -I. -c $< -o $@
			@echo -e "$@ done"

$(LIBDIR)/$(LIBPREFIX)%.$(DllSuf) :
			$(checkdir)
			@rm -f $@
			@$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@
			@chmod 555 $@
			@echo "==> $@ done"

# targets
all:		libs
include	$(patsubst %,%/$(MODMAKEFILE),$(MODULES))

libs:		updateinc $(ALLLIBS)

updateinc:	$(ALLHDRS) # only for linux
		@if [ ! -d $(INCDIR) ]; then mkdir -p $(INCDIR); fi
		@cp -v -u $^ $(INCDIR)

debug:		all

clean:
		@rm -fv $(DEPENDFILE) $(DEPENDFILE).bak \
		  $(TMPDIR)/*.so $(TMPDIR)/*.d # foreign build files

distclean:	clean

distsrc:
		@rm -f $(DISTSRCNAME).tar.xz;
		@tar --ignore-failed-read -cJvf $(DISTSRCNAME).tar.xz \
		  $(ALLDIST) *.C;
		@echo -e "\n$(DISTSRCNAME).tar.xz done\n"

$(DEPENDFILE):	$(ALLDEPEND)
		$(emptyfile)
		@$(MAKEDEPEND) -Y -f$@ -p$(dir $@) \
		-- $(CXXFLAGS) -- $^ 2>/dev/null
		@echo "$@ done"

ifeq (,$(findstring $(MAKECMDGOALS),$(NODEPEND)))
-include $(DEPENDFILE)
endif
