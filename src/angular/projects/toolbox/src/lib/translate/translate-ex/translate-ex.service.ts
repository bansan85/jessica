import { Inject, Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class TranslateExService {
  constructor(@Inject('languages') public languages: string[]) {}

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
}
