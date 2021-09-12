import { Component } from '@angular/core';
import { ComponentFixture, TestBed } from '@angular/core/testing';
import { MeyerhofCalcService } from 'jessica';

import { TranslateExService, TranslateMockPipe } from 'toolbox';

import { MainComponent } from './main.component';

@Component({ selector: 'lib-meyerhof-calc', template: '' })
class MeyerhofCalcStubComponent {}

describe('MainComponent', () => {
  let component: MainComponent;
  let fixture: ComponentFixture<MainComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [
        MainComponent,
        MeyerhofCalcStubComponent,
        TranslateMockPipe
      ]
    }).compileComponents();
  });

  beforeEach(() => {
    const spyTranslateEx = jasmine.createSpyObj(
      'TranslateExService',
      ['extractLanguage'],
      ['language']
    );
    const spyMeyerhofCalcService = jasmine.createSpyObj('MeyerhofCalcService', [
      'compute'
    ]);

    fixture = TestBed.overrideComponent(MainComponent, {
      set: {
        providers: [
          { provide: TranslateExService, useValue: spyTranslateEx },
          { provide: MeyerhofCalcService, useValue: spyMeyerhofCalcService }
        ]
      }
    }).createComponent(MainComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
