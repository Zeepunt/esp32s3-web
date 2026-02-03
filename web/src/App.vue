<script setup lang="ts">
</script>

<template>
  <div id="app">
    <h1>Welcome to the App</h1>
    <p>Authentication will be handled by browser's default login prompt.</p>
  </div>
</template>

<style>
body {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;
}

#app {
  width: 100%;
  height: 100vh;
  margin: 0;
  padding: 0;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  background-color: #f5f5f5;
}

#app h1 {
  color: #333;
  margin-bottom: 1rem;
}

#app p {
  color: #666;
  font-size: 1.1rem;
}
</style>
