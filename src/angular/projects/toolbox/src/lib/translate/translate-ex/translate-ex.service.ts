import { Inject, Injectable } from '@angular/core';

import likelySubtags from 'cldr-data/supplemental/likelySubtags.json';
import numberingSystems from 'cldr-data/supplemental/numberingSystems.json';

import globalize from 'globalize';
import 'globalize/number';

@Injectable({
  providedIn: 'root'
})
export class TranslateExService {
  public Globalize;

  private _language: string;

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
  }
}
