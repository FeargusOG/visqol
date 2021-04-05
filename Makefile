build:
	bazel build :visqol -c opt

build-debug:
	bazel build :visqol --compilation_mode=dbg

run-unit-tests:
	bazel test :all_unit_tests

run-conformance-tests:
	bazel test :conformance_test

run-guitar:
	./bazel-bin/visqol --reference_file ./testdata/conformance_testdata_subset/guitar48_stereo.wav --degraded_file ./testdata/conformance_testdata_subset/guitar48_stereo_64kbps_aac.wav

run-guitar-mmd:
	./bazel-bin/visqol --reference_file ./testdata/conformance_testdata_subset/guitar48_stereo.wav --degraded_file ./testdata/conformance_testdata_subset/guitar48_stereo_64kbps_aac.wav --use_memory_mapping true

profile-mem:
	bazel build :visqol --compilation_mode=dbg
	./scripts/profile-memory.sh