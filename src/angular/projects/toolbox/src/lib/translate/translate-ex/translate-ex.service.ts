import { Inject, Injectable } from '@angular/core';
import { AbstractControl, ValidationErrors, ValidatorFn } from '@angular/forms';

import likelySubtags from 'cldr-data/supplemental/likelySubtags.json';
import numberingSystems from 'cldr-data/supplemental/numberingSystems.json';

import globalize from 'globalize';
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
    this.Globalize = globalize;
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
    /* eslint-disable security/detect-object-injection, guard-for-in */
    if (i18n.child !== undefined) {
      for (const ch in i18n.child) {
        this.i18nStringToNumber(
          obj[i18n.child[ch]] as Record<string, unknown>,
          (i18n as Record<string, I18nForm>)[i18n.child[ch]],
          nbParse
        );
      }
    }
    if (i18n.number !== undefined) {
      for (const nb in i18n.number) {
        obj[i18n.number[nb]] = nbParse(obj[i18n.number[nb]] as string);
      }
    }
    /* eslint-enable security/detect-object-injection, guard-for-in */
  }

  i18nStringChangeLocale(
    obj: Record<string, unknown>,
    i18n: I18nForm,
    from: string,
    to: string
  ): void {
    /* eslint-disable security/detect-object-injection, guard-for-in */
    if (i18n.child !== undefined) {
      for (const ch in i18n.child) {
        this.i18nStringChangeLocale(
          obj[i18n.child[ch]] as Record<string, unknown>,
          (i18n as Record<string, I18nForm>)[i18n.child[ch]],
          from,
          to
        );
      }
    }
    if (i18n.number !== undefined) {
      this.Globalize.locale(from);
      const fromParser = this.Globalize.numberParser();
      this.Globalize.locale(to);
      const toParser = this.Globalize.numberFormatter();
      for (const nb in i18n.number) {
        obj[i18n.number[nb]] = toParser(
          fromParser(obj[i18n.number[nb]] as string)
        );
      }
    }
    /* eslint-enable security/detect-object-injection, guard-for-in */
  }
}
