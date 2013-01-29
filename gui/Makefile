# General settings
CP = cp
GREP = grep
GCOMP = lazbuild
OUTDIR = ..
GDIR = .
GOUT = ledborg_gui
GPROJ = $(GDIR)/$(GOUT).lpi

### Aliases ###
default:	$(GOUT)

rebuild:	clean $(GOUT)

### Clean ###
clean:
	$(RM) -r $(GDIR)/lib
	$(RM) $(GDIR)/*.ppu
	$(RM) $(GDIR)/*.o
	$(RM) $(GDIR)/$(GOUT)
	$(RM) $(OUTDIR)/$(GOUT)

### Output binaries ###
$(GOUT):	$(GPROJ) *.pas *.lfm
	$(GCOMP) $(GPROJ) | $(GREP) -v -e "Hint: Parameter \"Sender\" not used"
	$(CP) $(GDIR)/$(GOUT) $(OUTDIR)/$(GOUT)
