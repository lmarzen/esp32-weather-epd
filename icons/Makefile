SIZES := 16 24 32 48 64 96 128 160 196

# Generate the list of target directories based on sizes
TARGETS := $(foreach size, $(SIZES), icons/$(size)x$(size))

.PHONY: all clean final

all: $(TARGETS) final

icons/%:
	bash ./svg_to_headers.sh $(word 1, $(subst x, ,$*))

final: $(TARGETS)
	python3 final_generate_icons_h.py

clean:
	rm -rf png icons

