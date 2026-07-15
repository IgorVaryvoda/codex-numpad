.PHONY: test doctor install build flash dictation

test:
	bash tests/test-actions.sh
	bash -n bin/kbd-codex bin/kbd-herdr bin/kbd-dictate install.sh scripts/qmk-build scripts/setup-dictation tests/test-actions.sh

doctor:
	bin/kbd-codex doctor

install:
	./install.sh

build:
	./scripts/qmk-build build

flash:
	./scripts/qmk-build flash

dictation:
	./scripts/setup-dictation
