import { TestBed } from '@angular/core/testing';
import { FormControl, ValidatorFn } from '@angular/forms';
import { RouterTestingModule } from '@angular/router/testing';
import {
  TranslateFakeLoader,
  TranslateLoader,
  TranslateModule
} from '@ngx-translate/core';
import { AppComponent } from './app.component';

describe('AppComponent', () => {
  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [
        RouterTestingModule,
        TranslateModule.forRoot({
          loader: {
            provide: TranslateLoader,
            useClass: TranslateFakeLoader
          }
        })
      ],
      declarations: [AppComponent]
    }).compileComponents();
  });

  it('should create the app', () => {
    const fixture = TestBed.createComponent(AppComponent);
    const app = fixture.componentInstance;
    expect(app).toBeTruthy();
    expect(app.title).toEqual('app-main');
    app.changeLanguage('en-US');
    expect(app.translateEx.language).toBe('en');
  });

  it('validNumber', async () => {
    const fixture = TestBed.createComponent(AppComponent);
    const app = fixture.componentInstance;
    await app.changeLanguage('en-US');
    expect(app.translateEx.language).toBe('en');
    const validatorEn: ValidatorFn = app.translateEx.validNumber();
    expect(validatorEn(new FormControl('123456'))).toBeNull();
    expect(validatorEn(new FormControl('123.456'))).toBeNull();
    expect(validatorEn(new FormControl('123,456'))).toBeNull();
    await app.changeLanguage('fr-FR');
    expect(app.translateEx.language).toBe('fr');
    const validatorFr: ValidatorFn = app.translateEx.validNumber();
    expect(validatorFr(new FormControl('123456'))).toBeNull();
    expect(validatorFr(new FormControl('123.456'))).toEqual({
      validNumber: { reason: 'invalid number', value: '123.456' }
    });
    expect(validatorFr(new FormControl('123,456'))).toBeNull();
  });
});
