CONVERT=convert
XXD=xxd
TEXDIR=../

all: psds data

psds: \
$(TEXDIR)/charset.tga \
$(TEXDIR)/bunny.tga \
$(TEXDIR)/chicken.tga \
$(TEXDIR)/dinosaur.tga \
$(TEXDIR)/terrain.tga \
$(TEXDIR)/country.tga

data: \
$(DATADIR)/tweaks.dat \
$(DATADIR)/stages.dat

$(TEXDIR)/%.tga: textures/%.psd
	$(CONVERT) $< -depth 32 -flatten -alpha on +compress $@

$(DATADIR)/%.dat: data/%.hex
	$(XXD) $< -r > $@

clean:
	rm -f $(TEXDIR)/*.tga

