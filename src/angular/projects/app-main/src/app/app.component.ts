import { Component } from '@angular/core';
import { TranslateService } from '@ngx-translate/core';

import all_lang from './util/translate/all-lang.json';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'app-main';

  constructor(public translate: TranslateService) {
    translate.addLangs(all_lang);
    translate.setDefaultLang('en');
    translate.use(this.extractLanguage(navigator.language));
  }

  extractLanguage(language: string) {
    if (all_lang.indexOf(language) != -1) {
      console.log('LANG' + language);
      return language;
    }

    const pos = language.indexOf('-');
    const short_lang = pos === -1 ? language : language.substring(0, pos);

    if (all_lang.indexOf(short_lang) != -1) {
      console.log('LANG' + short_lang);
      return short_lang;
    }

    console.log('LANG' + 'en');
    return 'en';
  }
}
