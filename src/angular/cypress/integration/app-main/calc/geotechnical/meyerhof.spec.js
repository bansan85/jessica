describe('My First Test', () => {
  it('clicks the link "type"', () => {
    cy.visit('/');

    cy.contains('Width');
    cy.contains('Load');
    cy.contains('Form Values');

    cy.get('#width').type('1');
    cy.get('#load').type('100000');
    cy.get('#eccentric').type('0.25');

    cy.contains('"qref": 200000');
    cy.contains('"b_": 0.5');

    cy.get('#language').should('have.value', 'en');
    cy.get('#language').select('fr');
    cy.get('#language').should('have.value', 'fr');
  });
});
