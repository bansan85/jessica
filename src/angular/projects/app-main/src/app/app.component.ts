import { Component } from '@angular/core';
import { TranslateService } from '@ngx-translate/core';

import allLang from './util/translate/all-lang.json';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'app-main';

  constructor(public translate: TranslateService) {
    translate.addLangs(allLang);
    translate.setDefaultLang('en');
    translate.use(this.extractLanguage(navigator.language));
  }

  extractLanguage(language: string): string {
    if (allLang.indexOf(language) != -1) {
      console.log('LANG' + language);
      return language;
    }

    const pos = language.indexOf('-');
    const shortLang = pos === -1 ? language : language.substring(0, pos);

    if (allLang.indexOf(shortLang) != -1) {
      console.log('LANG' + shortLang);
      return shortLang;
    }

    console.log('LANG' + 'en');
    return 'en';
  }
}
