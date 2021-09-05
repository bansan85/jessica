import { TestBed } from '@angular/core/testing';

import { TranslateExService } from './translate-ex.service';

describe('TranslateExService', () => {
  let service: TranslateExService;

  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [{ provide: 'languages', useValue: ['en', 'fr', 'fr-FR'] }]
    });
    service = TestBed.inject(TranslateExService);
  });

  it('extractLanguage', () => {
    expect(service).toBeTruthy();
    expect(service.extractLanguage('en')).toBe('en');
    expect(service.extractLanguage('fr-FR')).toBe('fr-FR');
    expect(service.extractLanguage('fr')).toBe('fr');
    expect(service.extractLanguage('en-US')).toBe('en');
    expect(service.extractLanguage('fr-CA')).toBe('fr');
    expect(service.extractLanguage('en-UK')).toBe('en');
    expect(service.extractLanguage('de-DE')).toBe('en');
    expect(service.extractLanguage('de')).toBe('en');
  });
});
