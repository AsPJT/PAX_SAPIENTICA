# Workflow for GitHub Actions

## workflow
|workflow|description|trigger|
|---|---|---|
|[cmake-all-build.yml](.github/workflows/cmake-all-build.yml)|build all cmake projects|pull request|
|[cmake-unit-test.yml](.github/workflows/cmake-unit-test.yml)|run unit test for all cmake projects|pull request|
|[cppcheck.yml](.github/workflows/cppcheck.yml)|run cppcheck for all projects|pull request|
|[reviewdog-cpplint.yml](.github/workflows/reviewdog-cpplint.yml)|run cpplint for all projects|pull request|
