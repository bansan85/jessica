/* eslint-disable max-len */
import { ComponentFixture, TestBed } from '@angular/core/testing';
import { ReactiveFormsModule } from '@angular/forms';

import { TranslateExService, TranslateMockPipe } from 'toolbox';

import { MeyerhofCalcComponent } from './meyerhof-calc.component';
/* eslint-enable max-len */

describe('MeyerhofCalcComponent', () => {
  let component: MeyerhofCalcComponent;
  let fixture: ComponentFixture<MeyerhofCalcComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ReactiveFormsModule],
      declarations: [MeyerhofCalcComponent, TranslateMockPipe]
    }).compileComponents();
  });

  beforeEach(() => {
    const spy = jasmine.createSpyObj(
      'TranslateExService',
      ['extractLanguage'],
      ['language']
    );
    fixture = TestBed.overrideComponent(MeyerhofCalcComponent, {
      set: { providers: [{ provide: TranslateExService, useValue: spy }] }
    }).createComponent(MeyerhofCalcComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
