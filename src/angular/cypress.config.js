const { defineConfig } = require('cypress');

module.exports = defineConfig({
  e2e: {
    baseUrl: 'http://localhost:4200',
    reporter: 'junit',
    reporterOptions: {
      mochaFile: 'cypress.xml',
      toConsole: 'true'
    },
    supportFile: false,
    videoCompression: 15
  }
});
