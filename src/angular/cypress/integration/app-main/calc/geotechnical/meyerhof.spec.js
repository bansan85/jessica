describe('Auto English translation', () => {
  beforeEach(() => {
    cy.visit('/', {
      onBeforeLoad: (contentWindow) => {
        Object.defineProperty(contentWindow.navigator, 'language', {
          value: 'en'
        });
      }
    })
      .its('navigator.language')
      .should('equal', 'en');
  });

  it('Full form', () => {
    cy.contains('Width');
    cy.contains('Load');
    cy.contains('Form Values');

    cy.get('#width').type('1');
    cy.get('#load').type('100000');
    cy.get('#eccentric').type('0.25');

    cy.contains('qref: 200,000');
    cy.contains('b\': 0.5');

    cy.get('#language').should('have.value', 'en');
    cy.get('#language').select('fr');
    cy.get('#language').should('have.value', 'fr');

    cy.get('#eccentric').should('have.value', '0,25');
    cy.get('div').should('include.text', 'qref: 200\u202f000');
    cy.contains('b\': 0,5');

    cy.contains('Largeur');
  });

  it('Partial form', () => {
    cy.contains('Width');
    cy.contains('Load');
    cy.contains('Form Values');

    cy.get('#width').type('1.5');
    cy.get('#language').select('fr');
    cy.get('#language').should('have.value', 'fr');
    cy.get('#width').should('have.value', '1,5');
    cy.get('#language').select('en');
    cy.get('#language').should('have.value', 'en');
    cy.get('#width').should('have.value', '1.5');

    cy.get('#width').clear();
    cy.get('#eccentric').type('0.25');
    cy.get('#language').select('fr');
    cy.get('#eccentric').should('have.value', '0,25');
    cy.get('#language').select('en');
    cy.get('#eccentric').should('have.value', '0.25');
  });
});

describe('Auto French translation', () => {
  beforeEach(() => {
    cy.visit('/', {
      onBeforeLoad: (contentWindow) => {
        Object.defineProperty(contentWindow.navigator, 'language', {
          value: 'fr'
        });
      }
    })
      .its('navigator.language')
      .should('equal', 'fr');
  });

  it('Full form', () => {
    cy.contains('Largeur');
    cy.contains('Charge');
    cy.contains('Form Values');

    cy.get('#width').type('1');
    cy.get('#load').type('100000');
    cy.get('#eccentric').type('0,25');

    cy.get('div').should('include.text', 'qref: 200\u202f000');
    cy.contains('b\': 0,5');

    cy.get('#language').should('have.value', 'fr');
    cy.get('#language').select('en');
    cy.get('#language').should('have.value', 'en');

    cy.get('#eccentric').should('have.value', '0.25');
    cy.contains('qref: 200,000');
    cy.contains('b\': 0.5');

    cy.contains('Width');
  });
});
