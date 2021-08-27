/* eslint-disable max-len */
import { ComponentFixture, TestBed } from '@angular/core/testing';
import { ReactiveFormsModule } from '@angular/forms';

import { MeyerhofCalcComponent } from './meyerhof-calc.component';
import { TranslateMockPipe } from '../../../util/translation/translate-mock.pipe';
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
    fixture = TestBed.createComponent(MeyerhofCalcComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
