import { Inject, Injectable } from '@angular/core';
import { AbstractControl, ValidationErrors, ValidatorFn } from '@angular/forms';

import likelySubtags from 'cldr-data/supplemental/likelySubtags.json';
import numberingSystems from 'cldr-data/supplemental/numberingSystems.json';

import gglobalize from 'globalize';
import 'globalize/number';
import { I18nForm } from '../i18n-form';

@Injectable({
  providedIn: 'root'
})
export class TranslateExService {
  public Globalize;

  private _language: string;
  private _numberParser!: (value: string) => number;

  constructor(@Inject('languages') public languages: string[]) {
    this._language = 'en';
    this.Globalize = gglobalize;
    this.Globalize.load([likelySubtags, numberingSystems]);
  }

  extractLanguage(language: string): string {
    if (this.languages.indexOf(language) != -1) {
      return language;
    }

    const pos = language.indexOf('-');
    const shortLang = pos === -1 ? language : language.substring(0, pos);

    if (this.languages.indexOf(shortLang) != -1) {
      return shortLang;
    }

    return 'en';
  }

  get language(): string {
    return this._language;
  }

  set language(language: string) {
    this._language = language;
    this.Globalize.locale(language);
    this._numberParser = this.Globalize.numberParser();
  }

  get numberParser(): (value: string) => number {
    return this._numberParser;
  }

  validNumber(): ValidatorFn {
    return (control: AbstractControl): ValidationErrors | null => {
      // Before calling isNan, be sure value can't be empty or null.
      if (isNaN(this._numberParser(control.value))) {
        return {
          validNumber: {
            reason: 'invalid number',
            value: control.value
          }
        };
      }

      return null;
    };
  }

  i18nStringToNumber(
    obj: Record<string, unknown>,
    i18n: I18nForm,
    nbParse: (value: string) => number
  ): void {
    /* eslint-disable security/detect-object-injection */
    i18n.child?.map((ch: string) =>
      this.i18nStringToNumber(
        obj[ch] as Record<string, unknown>,
        (i18n as Record<string, I18nForm>)[ch],
        nbParse
      )
    );

    i18n.number?.map((nb: string) => (obj[nb] = nbParse(obj[nb] as string)));
    /* eslint-enable security/detect-object-injection */
  }

  i18nStringChangeLocale(
    obj: Record<string, unknown>,
    i18n: I18nForm,
    from: string | ((value: string) => number),
    to: string | ((value: number) => string)
  ): void {
    let parserFrom: (value: string) => number;
    if (typeof from === 'string') {
      this.Globalize.locale(from);
      parserFrom = this.Globalize.numberParser();
    } else {
      parserFrom = from;
    }
    let parserTo: (value: number) => string;
    if (typeof to === 'string') {
      this.Globalize.locale(to);
      parserTo = this.Globalize.numberFormatter();
    } else {
      parserTo = to;
    }

    /* eslint-disable security/detect-object-injection */
    i18n.child
      ?.filter((ch: string) => obj[ch] !== '')
      .map((ch: string) =>
        this.i18nStringChangeLocale(
          obj[ch] as Record<string, unknown>,
          (i18n as Record<string, I18nForm>)[ch],
          parserFrom,
          parserTo
        )
      );

    i18n.number
      ?.filter(
        (nb: string) =>
          obj[nb] !== null &&
          obj[nb] !== '' &&
          !Number.isNaN(parserFrom(obj[nb] as string))
      )
      .map((nb: string) => (obj[nb] = parserTo(parserFrom(obj[nb] as string))));
    /* eslint-enable security/detect-object-injection */
  }
}
