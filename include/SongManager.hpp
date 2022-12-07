namespace EndlessMode::SongManager
{
    // Enable or disable mod
    void SetShouldContinue(bool value);

    // Get if mod is enabled
    bool GetShouldContinue();

    // Collects our songs
    void CollectSongs();

    // Gets a new song and starts it
    void GetAndStartNextSong();
}