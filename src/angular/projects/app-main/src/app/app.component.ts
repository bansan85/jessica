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

  async changeLanguage(language: string): Promise<void> {
    const selectedLang = this.translateEx.extractLanguage(language);
    const locale = await import(
      /* webpackInclude: /(en|fr)\.mjs$/ */
      '../../node_modules/@angular/common/locales/' + selectedLang
    );
    registerLocaleData(locale.default, selectedLang);
    const cldrLocale = await import(
      /* webpackInclude: /main.(en|fr).numbers.json/ */
      'cldr-data/main/' + selectedLang + '/numbers.json'
    );
    this.translateEx.Globalize.load(cldrLocale.default);
    this.translate.use(selectedLang);
    // Subscribers to onLangChange may be called before or after the next
    // line.
    this.translateEx.language = selectedLang;
  }
}
