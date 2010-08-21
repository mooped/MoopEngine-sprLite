CONVERT=convert
TEXDIR=../

all: psds

psds: \
$(TEXDIR)/charset.tga \
$(TEXDIR)/sprites.tga

$(TEXDIR)/%.tga: textures/%.psd
	$(CONVERT) $< -depth 32 -flatten -alpha on +compress $@

clean:
	rm -f $(TEXDIR)/*.tga
