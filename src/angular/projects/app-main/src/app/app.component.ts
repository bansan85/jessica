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
      import(
        /* webpackInclude: /main.(en|fr).numbers.json/ */
        'cldr-data/main/' + selectedLang + '/numbers.json'
      ).then((cldrLocale) => {
        this.translateEx.Globalize.load(cldrLocale.default);
        this.translate.use(selectedLang);
        this.translateEx.language = selectedLang;
        this.translateEx.Globalize.locale(selectedLang);
        const parser = this.translateEx.Globalize.numberParser();
        console.log(parser('12,345.678'));
        console.log(parser('12 345,678'));
        console.log(parser('12345,678'));
      });
    });
  }
}
