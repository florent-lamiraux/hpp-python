name: update-flake-lock

on:
  workflow_dispatch: # allows manual triggering
  schedule:
    - cron: '0 0 * * 0' # runs weekly on Sunday at 00:00

jobs:
  lockfile:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout repository
        uses: actions/checkout@v4
      - name: Install Nix
        uses: DeterminateSystems/nix-installer-action@main
      - name: Update flake.lock
        uses: DeterminateSystems/update-flake-lock@main
