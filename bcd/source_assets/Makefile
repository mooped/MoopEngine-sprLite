CONVERT=convert
XXD=xxd
TEXDIR=../
DATADIR=../

all: psds data

psds: \
$(TEXDIR)/charset.tga \
$(TEXDIR)/bunny.tga \
$(TEXDIR)/chicken.tga \
$(TEXDIR)/dinosaur.tga \
$(TEXDIR)/terrain.tga \
$(TEXDIR)/country.tga \
$(TEXDIR)/title.tga \
$(TEXDIR)/pause.tga \
$(TEXDIR)/victory.tga \
$(TEXDIR)/defeat.tga

data: \
$(DATADIR)/tweaks.dat \
$(DATADIR)/stages.dat

$(TEXDIR)/%.tga: textures/%.psd
	$(CONVERT) $< -depth 32 -flatten -alpha on +compress $@

$(DATADIR)/%.dat: data/%.hex
	$(XXD) -r $< > $@
	$(XXD) $@ > $<

clean:
	rm -f $(TEXDIR)/*.tga
	rm -f $(DATADIR)/*.dat

