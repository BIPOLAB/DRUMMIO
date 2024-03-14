<script lang="ts">
  let searchTerms: string;

  interface SearchResult {
    title: string;
    url: string;
  }

  let searchResults: Promise<SearchResult[]>;

  async function search() {
    const duration = 2000; // equal 2s
    await new Promise((resolve) => setTimeout(resolve, duration));

    const dummyResults: SearchResult[] = [
      {
        title: "Samuel El-Borai",
        url: "sam.elborai.me",
      },
      {
        title: "Another Sam",
        url: "example.org/other-sam",
      },
    ];
    return dummyResults;
  }

  function handleClick() {
    if (searchTerms) searchResults = search();
  }
</script>

<header id="topmenu">
  <div class="actions">
    {#await searchResults}
      <input disabled type="text" bind:value={searchTerms} />
      <button disabled>Let's find out 🕵️‍♀️</button>
      <p>Awaiting a response ...</p>
    {:then}
      <input placeholder="Type your search term" bind:value={searchTerms} />
      <button on:click={handleClick}>Let's find out 🕵️‍♀️</button>

      {#if searchResults}
        <pre>{JSON.stringify(searchResults)}</pre>
      {/if}
    {/await}
  </div>
</header>

<style>
  pre {
    text-align: left;
    margin: auto;
    margin-top: 20px;
    padding: 30px;
    background-color: rgb(36, 36, 44);
    color: white;
    overflow: auto;
    width: 80%;
    border-radius: 4px;
  }

  .actions {
    text-align: center;
    padding-top: 4px;
    padding-bottom: 4px;
    background-color: rgb(239, 239, 239);
    border-bottom: 1px solid lightgrey;
    box-shadow: 0 0 4px #00000038;
  }

  .actions input,
  .actions button {
    margin: 0;
  }
</style>