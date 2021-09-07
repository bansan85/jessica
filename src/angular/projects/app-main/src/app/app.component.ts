import { registerLocaleData } from '@angular/common';
import { Component } from '@angular/core';
import { TranslateService } from '@ngx-translate/core';

import { TranslateExService } from 'toolbox';

import allLang from './util/translate/all-lang.json';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css'],
  providers: [TranslateExService, { provide: 'languages', useValue: allLang }]
})
export class AppComponent {
  title = 'app-main';

  constructor(
    public translate: TranslateService,
    public translateEx: TranslateExService
  ) {
    translate.addLangs(allLang);
    translate.setDefaultLang('en');
    this.changeLanguage(navigator.language);
  }

  changeLanguage(language: string): void {
    const selectedLang = this.translateEx.extractLanguage(language);
    import(
      /* webpackInclude: /(en|fr)\.js$/ */
      '@angular/common/locales/' + selectedLang
    ).then((locale) => {
      registerLocaleData(locale.default, selectedLang);
      this.translate.use(selectedLang);
      this.translateEx.language = selectedLang;
    });
  }
}
