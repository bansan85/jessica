import { TestBed } from '@angular/core/testing';
import { cloneDeep } from 'lodash-es';

import { I18nForm } from '../i18n-form';
import { TranslateExService } from './translate-ex.service';

interface Single<T extends number | string> {
  one: T;
}

class SingleImpl<T extends number | string> implements Single<T> {
  constructor(public one: T) {}
}

class SingleI18n implements I18nForm {
  public number: string[] = ['one'];
}

interface Double<T extends number | string> {
  one: T;
  two: T;
}

class DoubleImpl<T extends number | string> implements Double<T> {
  constructor(public one: T, public two: T) {}
}

class DoubleI18n implements I18nForm {
  public number: string[] = ['one', 'two'];
}

interface Form<T extends number | string> {
  child1: Single<T>;
  child2: Double<T>;
}

class FormImpl<T extends number | string> implements Form<T> {
  constructor(public child1: Single<T>, public child2: Double<T>) {}
}

class FormI18n implements I18nForm {
  public child1: SingleI18n = new SingleI18n();
  public child2: DoubleI18n = new DoubleI18n();
  public child: string[] = ['child1', 'child2'];
}

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

  it('i18n', () => {
    const formI18n: FormI18n = new FormI18n();
    const form: FormImpl<string> = new FormImpl<string>(
      new SingleImpl<string>('1.2'),
      new DoubleImpl<string>('3.2', 'hello')
    );

    const formStr: Form<string | number> = cloneDeep(form);

    expect(JSON.stringify(formStr)).toBe(
      '{"child1":{"one":"1.2"},"child2":{"one":"3.2","two":"hello"}}'
    );

    service.i18nStringToNumber(
      formStr as unknown as Record<string, unknown>,
      formI18n,
      parseFloat
    );

    // With i18nStringToNumber you lose non-number data.
    expect(JSON.stringify(formStr)).toBe(
      '{"child1":{"one":1.2},"child2":{"one":3.2,"two":null}}'
    );

    const formStr2: Form<string | number> = cloneDeep(form);

    service.i18nStringChangeLocale(
      formStr2 as unknown as Record<string, unknown>,
      formI18n,
      parseFloat,
      String
    );

    // With i18nStringChangeLocale you don't lose non-number data.
    expect(JSON.stringify(formStr2)).toBe(
      '{"child1":{"one":"1.2"},"child2":{"one":"3.2","two":"hello"}}'
    );
  });
});
